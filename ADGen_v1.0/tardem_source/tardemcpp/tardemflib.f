
c  Copyright (C) 2000  David Tarboton, Utah State University

c This program is free software; you can redistribute it and/or
c modify it under the terms of the GNU General Public License 
c version 2, 1991 as published by the Free Software Foundation.

c This program is distributed in the hope that it will be useful,
c but WITHOUT ANY WARRANTY; without even the implied warranty of
c MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
c GNU General Public License for more details.

c A copy of the full GNU General Public License is included in file 
c gpl.html. This is also available at:
c http://www.gnu.org/copyleft/gpl.html
c or from:
c The Free Software Foundation, Inc., 59 Temple Place - Suite 330, 
c Boston, MA  02111-1307, USA.

c If you wish to use or incorporate this program (or parts of it) into 
c other software that does not meet the GNU General Public License 
c conditions contact the author to request permission.
c David G. Tarboton                                                      
c Utah State University 
c 8200 Old Main Hill 
c Logan, UT 84322-8200 
c USA 
c http://www.engineering.usu.edu/dtarb/ 
c email:  dtarb@cc.usu.edu 
C
C   Subroutine to extract channel network from DEM area and pointer files
C
C       CREATED BY DAVID G TARBOTON
C       Utah State University
C
C    Based on the support area threshold this program identifies each link in
C    the channel network and sets up the tree file defining its topological
C    structure. The (I,J) row and column numbers of each pixel along each link
C    are saved for later reading by NETPROP.FOR. 
C 
	

       subroutine NETEX(dir,area,treefile,coordfile,ordfile,nx,ny,
     +   itresh,icr,icend,dx,dy,bndbox,csize,iftype,err)
	!DEC$ ATTRIBUTES DLLEXPORT::NETEX

       PARAMETER(MNL=50000) 
c      PARAMETER(IGX=100,IGY=100,MNL=5000) 
      CHARACTER*4096 TREEFILE,COORDFILE, ordfile, pointfile,areafile
C---with MNL = 50000 the number of links can exceed 32767 the max two
C    bit integer value so link pointers have to be 4 bit integers
C     (NEXTL,PREVL1,PREVL2,IPOINT)
      INTEGER AREA(ny,nx),NEXTL(MNL),PREVL1(MNL),PREVL2(MNL),IPOINT(7)
      INTEGER*2 DIR(ny,nx),D1(8),D2(8),IST((MNL+1)/2),JST((MNL+1)/2),
     &IORD(MNL),err,
     &ISTART(MNL),JSTART(MNL),IEND(MNL),JEND(MNL),MAG(MNL),IORDUP(7)
	parameter(ishrdt = 1, iintdt = 2, ifltdt=3)
	double precision BNDBOX(4),csize
      LOGICAL strt
      DATA D1/0,-1,-1,-1,0,1,1,1/
      DATA D2/1,1,0,-1,-1,-1,0,1/
      
C
C    READ INPUT
C
      igy=ny
	igx=nx
	err=0
C
C       MEANING OF POINTERS IS -------------
C                              I 4 I 3 I 2 I
C        0 = POINTS TO SELF    -------------
C            I.E. UNRESOLVED   I 5 I 0 I 1 I
C        -1= BOUNDARY PIXEL    -------------
C                              I 6 I 7 I 8 I
C                              -------------                      
C
C----FIRST FIND ALL START PIXELS
C
      N=0 
c	open(8,file="c:\tt.dat")          
      nmax=(mnl+1)/2
      DO 2 I=1,NY
        DO 2 J=1,NX                                             
          IF(strt(I,J,AREA,DIR,NX,NY,IGX,IGY,ITRESH))THEN
            N=N+1
            if(n.le.nmax)then
            IST(N)=I
            JST(N)=J
            endif
          ENDIF       
 2    CONTINUE   
c      WRITE(8,*)'MAGNITUDE',N
      if(n.gt.nmax) then
		err=2 !stop 'too big'
	    RETURN
	endif                   
C
C---ZERO AREA ARRAY
C
      DO 12 I=1,NY
       DO 12 J=1,NX
 12      AREA(I,J)=0
C
C----TRACE STREAMS DOWNWARDS ADDING 1 TO MAGNITUDE OF EACH PIXEL
C      (MAGNITUDE STORED IN AREA ARRAY)
      DO 3 IS=1,N
        I=IST(IS)
        J=JST(IS)
c	write(8,*)i,j
 4      IF(DIR(I,J).GT.0)THEN
          AREA(I,J)=AREA(I,J)+1
          INEXT=I+D1(DIR(I,J))
          JNEXT=J+D2(DIR(I,J))
          I=INEXT
          J=JNEXT
c	x=dx*j+bndbox(1)-dx/2.
c	y=bndbox(4)-dy*i+dy/2.
c	write(8,*)i,j,x,y
          GO TO 4
        ENDIF
 3    CONTINUE
C
C----IDENTIFY LINKS BY DIFFERENT MAGNITUDES
C
      ILINK=1
      DO 5 IS=1,N
        ISTART(ILINK)=IST(IS)
        JSTART(ILINK)=JST(IS)
C---INITIALISE POINTERS
        PREVL1(ILINK)=0
        PREVL2(ILINK)=0      
        I=IST(IS)
        J=JST(IS)
        MAG(ILINK)=AREA(I,J)
        IORD(ILINK)=1
 6      INEXT=I+D1(ABS(DIR(I,J)))
        JNEXT=J+D2(ABS(DIR(I,J)))
C----CHECK FOR END
        IF(DIR(INEXT,JNEXT).EQ.0)GO TO 90  ! END
        MNEXT=AREA(INEXT,JNEXT)
        I=INEXT
        J=JNEXT
        IEND(ILINK)=I
        JEND(ILINK)=J
        IF(MNEXT.EQ.MAG(ILINK))GO TO 6
C---CONTINUE HERE FOR NEW LINK
C----CHECK IF JUNCTION ALREADY REACHED (FLAGGED BY NEGATIVE DIRECTION)
          IF(DIR(I,J).LT.0)THEN
C----CHECK IF ALL LINKS CONVERGING HERE HAVE BEEN DONE BY SUMMING
C           MAGNITUDE
            MSUM=0
            IORDM=0
            ICONV=0
            DO 7 IL=1,ILINK
              IF(IEND(IL).EQ.I.AND.JEND(IL).EQ.J)THEN
                ICONV=ICONV+1      ! COUNTER OF NUMBER OF LINKS THAT CONVERGE
                IPOINT(ICONV)=IL
                IORDUP(ICONV)=IORD(IL)
                MSUM=MSUM+MAG(IL)                  
              ENDIF
  7         CONTINUE
            IF(MSUM.EQ.MNEXT)THEN     ! YES ALL LINKS HAVE BEEN PROCESSED
C---SORT IORDUP,IPOINT INTO DECENDING STREAM ORDER
              DO 11 IC=1,ICONV-1
                DO 11 IIC=IC+1,ICONV
                  IF(IORDUP(IIC).GT.IORDUP(IC))THEN   ! SWITCH THESE
                    ITEMP=IORDUP(IIC)
                    IORDUP(IIC)=IORDUP(IC)
                    IORDUP(IC)=ITEMP
                    ITEMP=IPOINT(IIC)
                    IPOINT(IIC)=IPOINT(IC)
                    IPOINT(IC)=ITEMP
                  ENDIF
 11           CONTINUE
              DO 17 IC=1,ICONV-1
                ILINK=ILINK+1
                ISTART(ILINK)=I
                JSTART(ILINK)=J  
                PREVL1(ILINK)=IPOINT(IC)
                PREVL2(ILINK)=IPOINT(IC+1)
                NEXTL(IPOINT(IC))=ILINK
                NEXTL(IPOINT(IC+1))=ILINK
                MAG(ILINK)=MAG(PREVL1(ILINK))+MAG(PREVL2(ILINK))
                IORD(ILINK)=MAX(IORDUP(1),IORDUP(2)+1)
                IPOINT(IC+1)=ILINK
                IEND(ILINK)=I
                JEND(ILINK)=J
 17           CONTINUE
              GO TO 6       ! CONTINUE TRACING DOWN
            ELSE
              ILINK=ILINK+1
            ENDIF
          ELSE
            DIR(I,J)=-DIR(I,J)
            ILINK=ILINK+1
          ENDIF
 5    CONTINUE   
	err = 1 
	RETURN
cc      WRITE(6,*)'LOGIC ERROR LINK LOOP ENDED'
      STOP
C
C----WRITE TREE FILE
C     
 90   OPEN(UNIT=10,FILE=COORDFILE,STATUS='unknown')
      OPEN(UNIT=12,FILE=TREEFILE,STATUS='unknown') 
c
c   reinitialize area array - for output it will contain order
c	
      do 73 i=1,ny
	  do 73 j = 1,nx
 73       area(i,j)=0
      ICORD=0
C---WRITE ROOT LINK FIRST
      DO 10 IL=ILINK,ILINK
        I=ISTART(IL)
        J=JSTART(IL)
        ICS=ICORD
 20     WRITE(10,*)I,J
        area(i,j)=max(iord(il),area(i,j))
        ICEND=ICORD
        ICORD=ICORD+1
        IF(.NOT.(I.EQ.IEND(IL).AND.J.EQ.JEND(IL)))THEN
          INEXT=I+D1(ABS(DIR(I,J)))
          JNEXT=J+D2(ABS(DIR(I,J)))
          I=INEXT
          J=JNEXT
          GO TO 20
        ENDIF
        WRITE(12,122)0,ICS,ICEND,-1,PREVL1(IL)
     &  ,PREVL2(IL),IORD(IL)
 10   CONTINUE         
c      WRITE(6,*)'ROOT AT COORD. NUMBER ',ICORD
	icr=icord
c      WRITE(11,*)ICORD
C---WRITE REMAINDER OF LINKS
      DO 110 IL=1,ILINK-1
        I=ISTART(IL)
        J=JSTART(IL)
        ICS=ICORD	
 120     WRITE(10,*)I,J
         area(i,j)=max(iord(il),area(i,j))
        ICEND=ICORD
        ICORD=ICORD+1
        IF(.NOT.(I.EQ.IEND(IL).AND.J.EQ.JEND(IL)))THEN
          INEXT=I+D1(ABS(DIR(I,J)))
          JNEXT=J+D2(ABS(DIR(I,J)))
          I=INEXT
          J=JNEXT
          GO TO 120
        ENDIF
        IF(NEXTL(IL).EQ.ILINK)NEXTL(IL)=0
        WRITE(12,122)IL,ICS,ICEND,NEXTL(IL),PREVL1(IL)
     &  ,PREVL2(IL),IORD(IL)
 122    FORMAT(1X,7I9)
 110   CONTINUE
c       write(11,*)icend
      close(10)
	close(12)
cc	call fgridwrite(ordfile,area,2,nx,ny,dx,dy,bndbox,csize,-1.,
cc     &iftype,igy)
      END                                      
C
C     THIS FUNCTION RETURNS TRUE IF THE PIXEL IS A STREAM SOURCE
C      ACCORDING TO THE THRESHOLD
C          
      LOGICAL FUNCTION strt(I,J,AREA,DIR,NX,NY,IGX,IGY,ITRESH)
      INTEGER*2 DIR(IGY,IGX)
      INTEGER AREA(IGY,IGX),D1(8),D2(8)
      DATA D1/0,-1,-1,-1,0,1,1,1/
      DATA D2/1,1,0,-1,-1,-1,0,1/
      strt=.TRUE.   
      IF(AREA(I,J).LT.ITRESH)THEN
        strt=.FALSE.
        IF(AREA(I,J).LE.0)DIR(I,J)=0   ! ZERO DIRECTIONS OUTSIDE AREA
      ELSE    ! CHECK UPSTREAM PIXELS 
        DO 2 K=1,8
          IN=I+D1(K)    ! NEIGHBOUR PIXEL
          JN=J+D2(K)       
          IF(DIR(IN,JN).GT.0)THEN
          IND=IN+D1(DIR(IN,JN))   ! PIXEL DOWNSTREAM FROM NEIGHBOUR
          JND=JN+D2(DIR(IN,JN))
          IF(IND.EQ.I.AND.JND.EQ.J)THEN   !NEIGHBOUR DRAINS INTO (I,J)
            IF(AREA(IN,JN).GE.ITRESH)strt=.FALSE.
          ENDIF
          ENDIF
 2      CONTINUE
      ENDIF
      RETURN
      END     
C                                   
C
        
C   Subroutine to compute channel network properties for each pixel on the channel
C   network
C
C       CREATED BY DAVID G TARBOTON
C
C
C    This subroutine follows after NETEX.FOR and computes the X and Y coordinates
C    of pixels on the channel network from row and column numbers.
C    It also writes elevations, distances to
C    the outlet along the streams and contributing area for each pixel to the
C    coordinate file.
C
C
       subroutine NETPROP(dir,area,elev,coordfile,
     + icr,icmax,dx,dy,nx,ny,bndbox,csize,err)
	!DEC$ ATTRIBUTES DLLEXPORT::NETPROP
       PARAMETER(MC=500000)
c      parameter(ishrdt = 1, iintdt = 2, ifltdt=3) 
       CHARACTER*4096 COORDFILE,ELEVFILE,AREAFILE,POINTFILE
      INTEGER AREA(ny,nx),err
      REAL RAREA(MC),LENGTH(MC)
      real elev(ny,nx),elv(mc)
      INTEGER*2 DIR(ny,nx),D1(8),D2(8),IA(MC),JA(MC)
      double precision BNDBOX(4),csize
      DATA D1/0,-1,-1,-1,0,1,1,1/
      DATA D2/1,1,0,-1,-1,-1,0,1/
C
C    READ INPUT
C
c     READ(11,*)ICR       ! ICR IS COORD OF ROOT
c       read(11,*)icmax
       if(icmax.gt.mc)then
		err=1
		RETURN
         write(6,*)'Coord file dimension not big enough'
         stop
       endif
       OPEN(UNIT=10,FILE=coordfile,STATUS='OLD')
       DO 5 N=1,MC
       READ(10,*,END=110)IA(N),JA(N)
 5     CONTINUE
	 
CC      WRITE(6,*)'TOO MANY COORDS'
 110   CLOSE(10)
       N=N-1              
C
C---COMPUTE AREAS
C
C       CALL demread(AREA,AREAFILE,NX,NY,IGY,dx,dy)
c        call fgridread(AREAFILE,AREA, IINTDT, NX, NY,
c     &	 DX, DY, BNDBOX,CSIZE, XNDV,IFTYPE, IGY)
       DO 6 IC=1,N
 6       RAREA(IC)=FLOAT(AREA(IA(IC),JA(IC)))*dx*dy
C
C---READ ELEVATIONS
C
C      CALL demread(DIR,ELEVFILE,NX,NY,IGY,dx,dy)
c       call fgridread(ELEVFILE,elev, IfltDT, NX, NY,
c     &	 DX, DY, BNDBOX,CSIZE, XNDV,IFTYPE, IGY)

       DO 7 IC=1,N
         ELV(IC)=elev(IA(IC),JA(IC))
 7     CONTINUE 
       IROOT=IA(ICR)
       JROOT=JA(ICR)
C
C---READ POINTERS AND COMPUTE LENGTHS
C
C       CALL demread(DIR,POINTFILE,NX,NY,IGY,dx,dy)
c	   call fgridread(POINTFILE,DIR, ISHRDT, NX, NY,
c     &	 DX, DY, BNDBOX,CSIZE, XNDV,IFTYPE, IGY)

C
C       MEANING OF POINTERS IS -------------
C                              I 4 I 3 I 2 I
C        0 = POINTS TO SELF    -------------
C            I.E. UNRESOLVED   I 5 I 0 I 1 I
C        -1= BOUNDARY PIXEL    -------------
C                              I 6 I 7 I 8 I
C                              -------------                      
C
C----TRACE STREAMS DOWNWARDS
      DO 3 IC=1,N
        LENGTH(IC)=0.
        I=IA(IC)
        J=JA(IC)
        IF(.NOT.(I.EQ.IROOT.AND.J.EQ.JROOT))THEN      ! LOOP
 4      INEXT=I+D1(DIR(I,J))
        JNEXT=J+D2(DIR(I,J)) 
        DXx=dx*FLOAT(J-JNEXT)
        DYy=dy*FLOAT(I-INEXT)
        LENGTH(IC)=LENGTH(IC)+SQRT(DXx*DXx+DYy*DYy)
        IF(.NOT.(INEXT.EQ.IROOT.AND.JNEXT.EQ.JROOT))THEN
          I=INEXT
          J=JNEXT
          GO TO 4
        ENDIF
        ENDIF
 3    CONTINUE
C--WRITE OUTPUT
      OPEN(UNIT=10,FILE=COORDFILE,STATUS='UNKNOWN')
      DO 10 IC=1,N
        X=(JA(IC)-1)*dx+bndbox(1)+dx*0.5
        Y=dy*(NY-IA(IC))+bndbox(2)+dy*0.5
        WRITE(10,*)X,Y,LENGTH(IC),ELV(IC),RAREA(IC)
 10   CONTINUE
	close(10)
      END                                      
C
C      Subroutine TO ANALYSE LINKS OF CHANNEL NETWORK TREE STRUCTURE
C       CREATED BY DAVID G TARBOTON
c       Utah State University
c
C       THIS VERSION EXTRACTS NETWORK BASED ON Order
c       The value returned is the link magnitude
c     
c       The purpose of this routine as used here is to construct the TREE array
c       which defines the subset of the network that will be used.
c
C
       function LINKSOURCE(cnet,tree,coord,mag,iord,source,nlmax,nc,
     & iordt,err)
	!DEC$ ATTRIBUTES DLLEXPORT::LINKSOURCE
       INTEGER CNET(0:nlmax,7),TREE(nlmax,5),SOURCE(nlmax),mag(nlmax)
     & ,iord(nlmax),
     & F,FOUND,FOUNDT,err       
       REAL COORD(0:nc,5)

C  
C     DATA STRUCTURE:
C     ---------------
C     COORD CONTAINS DIGITISED POINTS AS FOLLOWS:
C     COORD(*,1) = X COORDINATE (metres)
C       "    ,2    Y    "
C       "    ,3    DISTANCE ALONG CHANNELS TO GAUGE (metres)
C       "    ,4    ELEVATION (tenths of metres)          
C       "    ,5    area M**2
C                                                
C     CNET CONTAINS LINKS AS STRINGS OF DIGITISED POINTS AS FOLLOWS:
C     CNET(*,1) = LINK NUMBER                                       
C       "   ,2    START POINT NUMBER IN COORD
C       "   ,3    END POINT NUMBER IN COORD
C       "   ,4    NEXT (DOWNSTREAM) LINK NUMBER IN CNET
C       "   ,5&6  PREVIOUS (UPSTREAM) LINK NUMBERS IN CNET
C       "   ,7    STRAHLER ORDER OF LINK IN CNET
C
C     TREE CONTAINS THE NETWORK AS BUILT UP DURING EXECUTION
C     TREE(*,1) = LAST LINK NUMBER (POINTS TO CNET)
C     TREE(*,2) = FIRST LINK NUMBER (POINTS TO CNET)
C       "   ,3    NEXT (DOWNSTREAM) LINK (POINTS TO TREE)
C       "   ,4&5  PREVIOUS (U.S.) LINKS (POINTS TO TREE) (0 WHEN NOT FULL)
C
C     SOURCE CONTAINS LINK NUMBERS (POINTS TO TREE) OF FIRST ORDER STREAMS
C      STILL OPEN I.E. MAY BIFURCATE UPSTREAM AT HIGHER ELEVATION
C 
C     E CONTAINS ELEVATIONS CORRESPONDING TO MAGNITUDE (ELEVATION AT WHICH
C       BIFURCATION TO THAT MAGNITUDE OCCURS WHEN WORKING UPWARDS)
C
c 	write(6,*)nlmax,iordt
c	do 2 j=0,nlmax-1
c	  write(6,*)(cnet(j,i),i=1,7)
c 2    continue
 
       NL=Nlmax-1     
c         OPEN(UNIT=12,FILE=OUTFILE,STATUS='NEW')
C       WRITE(6,*)'INPUT MAGNITUDE AND START LINK'
c       read(9,*)   ! skip lines
c       read(9,*)
c       READ(9,*)NMAX,ILINK,ISUP,APIX
       nmax = (nlmax+1)/2
c      READ(9,*)ILINK,ISUP,apix
c       AMIN=ISUP*APIX
      ilink=0
C                                         
C---INITIALISE
	 err=0        
       N=1
       NS=1             ! SOURCE SET POINTER
       SOURCE(1)=1      ! SOURCE SET INITIALLY ROOT LINK
       IT=1             ! TREE POINTER
       DO J=1,2*NMAX-1          ! INITIALISE TREE ARRAY TO ZERO
         DO K=1,5
           TREE(J,K)=0
         ENDDO
       ENDDO
       TREE(1,1)=ILINK           ! TREE INITIALLY ROOT LINK STARTS FROM
       TREE(1,2)=ILINK           ! POSITION 1, VALUE ZERO INDICATES NO BRANCH
c       E(N)=COORD(CNET(TREE(1,1),3),5)
       DO WHILE (NS.GT.0.AND.N.LE.NMAX)    ! SOURCE SET NOT EMPTY 
C---SEARCH SOURCE SET FOR LINK WITH highest UPSTREAM area
         F=1
         DO J=2,NS
           IF(COORD(CNET(TREE(SOURCE(J),2),2),5).GT.
     &     COORD(CNET(TREE(SOURCE(F),2),2),5))F=J
         ENDDO                 
         FOUNDT=SOURCE(F)    ! LINK IN TREE         
         FOUND=TREE(FOUNDT,2)! LINK IN CNET
C---REMOVE THIS LINK FROM SOURCE
c         ELEV=COORD(CNET(FOUND,2),5)
         CALL REMOVE(SOURCE,F,NS)
		if(err.ne.0)return
C---IF THERE ARE LINKS UPSTREAM OF F ADD THEM TO TREE AND SOURCE SET
         IF(CNET(FOUND,5).GT.0)THEN
          LINK1=CNET(FOUND,5)
c          ICRD1=MAX(CNET(LINK1,3)-1,CNET(LINK1,2))
          iord1=cnet(link1,7)
          LINK2=CNET(FOUND,6)
c          ICRD2=MAX(CNET(LINK2,3)-1,CNET(LINK2,2))
          iord2=cnet(link2,7)
          IF(iord1.GE.iordt.AND.iord2.GE.iordt)THEN
C---ADD LINKS UPSTREAM OF FOUND TO TREE
           IT=IT+1
           CALL ADD(SOURCE,IT,NS)
           TREE(IT,1)=CNET(FOUND,5)
           TREE(IT,2)=CNET(FOUND,5)
           TREE(IT,3)=FOUNDT
           TREE(FOUNDT,4)=IT
           IT=IT+1
           CALL ADD(SOURCE,IT,NS)
           TREE(IT,1)=CNET(FOUND,6)
           TREE(IT,2)=CNET(FOUND,6)
           TREE(IT,3)=FOUNDT
           TREE(FOUNDT,5)=IT
           N=N+1
c           E(N)=ELEV    
          ELSE IF(iord1.GE.iordt)THEN
           TREE(FOUNDT,2)=LINK1
           CALL ADD(SOURCE,FOUNDT,NS)
          ELSE IF(iord2.GE.iordt)THEN
           TREE(FOUNDT,2)=LINK2
           CALL ADD(SOURCE,FOUNDT,NS)
          ENDIF
         ENDIF
       ENDDO   
	 
c	 do i = 1,nlmax
c	  write(6,*)(tree(i,j),j=1,5)  
c	  enddo     
           CALL ANAL5(TREE,IT,COORD,CNET,N,ISUP,AMIN,nc,nlmax,mag,iord)
c       WRITE(6,*)'FINAL MAGNITUDE =',N
       linksource = n
	 return
       END   
C
C      SUBROUTINE TO ANALYSE TREE STRUCTURE
C                                           
      SUBROUTINE ANAL5(TREE,IT,COORD,CNET,N,ISUP,AMIN,nc,nlmax,mag,iord)
c      parameter(nlmax=50000,nc=500000)   !  remember to change subroutines
      INTEGER CNET(0:nlmax,7),TREE(nlmax,5),MAG(nlmax),
     &IORD(nlmax)
      REAL COORD(0:nc,5),HEIGHT,DELEV,LENGTH,
     &GLEN,DAREA                               
      LOGICAL DONE
C    
C    SCAN THROUGH TREE COMPUTING GENERATION, HEIGHT, LENGTH,
C     GEOMETRIC LENGTH, DOWNSTREAM ELEVATION, AND MAGNITUDE OF EACH LINK
C
C---COMPUTE MAGNITUDE AND ORDER
C
C----INITIALISE
      DO 3 I=1,IT
        IORD(I)=0
 3      MAG(I)=0
 1    DONE=.TRUE.
      DO 4 I=1,IT 
       IF(MAG(I).EQ.0)THEN
        IUP1=TREE(I,4)
        IUP2=TREE(I,5)
        IF(IUP1.EQ.0.AND.IUP2.EQ.0)THEN
          MAG(I)=1
          IORD(I)=1
        ELSE IF(MAG(IUP1).GT.0.AND.MAG(IUP2).GT.0)THEN
          MAG(I)=MAG(IUP1)+MAG(IUP2)
          IOMAX=MAX(IORD(IUP1),IORD(IUP2))
          IOMIN=MIN(IORD(IUP1),IORD(IUP2))
          IORD(I)=MAX(IOMAX,IOMIN+1)
        ELSE    ! PASS FOR NOW              
          DONE=.FALSE.
        ENDIF
       ENDIF
 4    CONTINUE        
      IF(.NOT.DONE)GO TO 1
C--WRITE HEADERS
c      WRITE(12,144)ISUP,AMIN,TREE(1,1)
c 144  FORMAT(1X,I5,E12.4,'  LINK DATA FROM LINK',I6/1X,'10'/
c     &1X,'LINK NO'/1X,'GENERATION'/1X,'LINK DROP'/1X,'LENGTH'/
c     &1X,'G. LENGTH'/1X,'ELEVATION'/1X,'STRAHLER ORDER'/1X,'MAGNITUDE'/
c     &1X,'AREA'/1X,'SLOPE')
      DO 2 I=1,IT 
C---HEIGHTS
        LINKE=TREE(I,1)   ! END LINK NO IN CNET
        LINKS=TREE(I,2)   ! START LINK NO IN CNET
        IUP=CNET(LINKS,2) ! UPSTREAM POINT IN COORD
C---SEARCH DOWN LINK FOR AREA .GE. AMIN
 10     IF(COORD(IUP,5).LT.AMIN)THEN
          IUP=IUP+1
          GO TO 10
        ENDIF
        ID=CNET(LINKE,3)  ! DOWNSTREAM PT IN COORD
        HEIGHT=COORD(IUP,4)-COORD(ID,4)
        LENGTH=COORD(IUP,3)-COORD(ID,3)
        DX=COORD(IUP,1)-COORD(ID,1)
        DY=COORD(IUP,2)-COORD(ID,2)
        GLEN=SQRT(DX*DX+DY*DY)
        DELEV=COORD(ID,4)
C---OBTAIN AREA FROM PIXEL JUST BEFORE END OF LINK SINCE PIXEL AT END OF
C    LINK ALSO CONTAINS AREA OF MERGING PIXELS
C
        IARP=MAX(IUP,ID-1)     
        DAREA=COORD(IARP,5)   
        SLOPE=HEIGHT/MAX(LENGTH,.0001)
        RNO=LINKS+LINKE/100000.
c        WRITE(12,100)RNO,HEIGHT,LENGTH,GLEN,DELEV
c     &  ,IORD(I),MAG(I),DAREA,SLOPE
c 100    FORMAT(1X,F12.5,4F9.2,2I6,2E12.5)
 2    CONTINUE
      RETURN                         
      END              
C
C      PROGRAM TO ANALYSE LINKS OF CHANNEL NETWORK TREE STRUCTURE
C       CREATED BY DAVID G TARBOTON
C       THIS VERSION EXTRACTS NETWORK BASED ON MIN SUPPORT AREA
c       and outputs in ARC e00 format
c       2-12-98
C
       subroutine ARCLINKS(coordfile,treefile,outfile,ilink,amin,err)
	!DEC$ ATTRIBUTES DLLEXPORT::ARCLINKS
       parameter(nlmax=50000,nc=500000)   !  remember to change subroutines
       INTEGER CNET(0:nlmax,7),TREE(nlmax,5),SOURCE(nlmax/2+1),
     & F,FOUND,FOUNDT,err       
       REAL COORD(0:nc,5)
       CHARACTER*4096 COORDFILE,TREEFILE,OUTFILE
C  
C     DATA STRUCTURE:
C     ---------------
C     COORD CONTAINS DIGITISED POINTS AS FOLLOWS:
C     COORD(*,1) = X COORDINATE (metres)
C       "    ,2    Y    "
C       "    ,3    DISTANCE ALONG CHANNELS TO GAUGE (metres)
C       "    ,4    ELEVATION (tenths of metres)          
C       "    ,5    area M**2
C                                                
C     CNET CONTAINS LINKS AS STRINGS OF DIGITISED POINTS AS FOLLOWS:
C     CNET(*,1) = LINK NUMBER                                       
C       "   ,2    START POINT NUMBER IN COORD
C       "   ,3    END POINT NUMBER IN COORD
C       "   ,4    NEXT (DOWNSTREAM) LINK NUMBER IN CNET
C       "   ,5&6  PREVIOUS (UPSTREAM) LINK NUMBERS IN CNET
C       "   ,7    STRAHLER ORDER OF LINK IN CNET
C
C     TREE CONTAINS THE NETWORK AS BUILT UP DURING EXECUTION
C     TREE(*,1) = LAST LINK NUMBER (POINTS TO CNET)
C     TREE(*,2) = FIRST LINK NUMBER (POINTS TO CNET)
C       "   ,3    NEXT (DOWNSTREAM) LINK (POINTS TO TREE)
C       "   ,4&5  PREVIOUS (U.S.) LINKS (POINTS TO TREE) (0 WHEN NOT FULL)
C
C     SOURCE CONTAINS LINK NUMBERS (POINTS TO TREE) OF FIRST ORDER STREAMS
C      STILL OPEN I.E. MAY BIFURCATE UPSTREAM AT HIGHER ELEVATION
C 
c       OPEN(UNIT=9,FILE='netex.in',STATUS='OLD',readonly)
c       READ(9,10)TREEFILE,COORDFILE,OUTFILE
c 10    FORMAT(///////A80//A80/A80)
         OPEN(UNIT=11,FILE=TREEFILE,STATUS='OLD',readonly)
	err = 0
C---READ IN LINKS
       DO NL=0,nlmax
         READ(11,*,END=333)(CNET(NL,I),I=1,7)
       ENDDO
c --- upon exiting this loop, nl contains the number of links read
cc       WRITE(6,*)'TOO MANY LINKS'
	   err=1
 333   CLOSE(11)
         OPEN(UNIT=10,FILE=COORDFILE,STATUS='OLD',readonly)
C---READ IN COORDS AND ELEVATIONS
       DO I=0,nc
         READ(10,*,END=444)(COORD(I,J),J=1,5)
       ENDDO
	   err=2
cc       WRITE(6,*)'TOO MANY CO ORDS'
C--- GET MAX MAGNITUDE AND START LINK
 444   CONTINUE                   
       close(10)
	 if(err.ne.0) return
         OPEN(UNIT=12,FILE=OUTFILE,STATUS='unknown')
C       WRITE(6,*)'INPUT MAGNITUDE AND START LINK'
c       read(9,*)   ! skip lines
c       read(9,*)
c       READ(9,*)NMAX,ILINK,ISUP,APIX
       nmax = (nl-1)/2+1
c       READ(9,*)ILINK,ISUP,apix
c       AMIN=ISUP*APIX
C                                         
C---INITIALISE        
       N=1
       NS=1             ! SOURCE SET POINTER
       SOURCE(1)=1      ! SOURCE SET INITIALLY ROOT LINK
       IT=1             ! TREE POINTER
       DO J=1,nl          ! INITIALISE TREE ARRAY TO ZERO
         DO K=1,5
           TREE(J,K)=0
         ENDDO
       ENDDO
       TREE(1,1)=ILINK           ! TREE INITIALLY ROOT LINK STARTS FROM
       TREE(1,2)=ILINK           ! POSITION 1, VALUE ZERO INDICATES NO BRANCH
       DO WHILE (NS.GT.0)    ! SOURCE SET NOT EMPTY 
C---SEARCH SOURCE SET FOR LINK WITH highest UPSTREAM area
         F=1
         DO J=2,NS
           IF(COORD(CNET(TREE(SOURCE(J),2),2),5).GT.
     &     COORD(CNET(TREE(SOURCE(F),2),2),5))F=J
         ENDDO                 
         FOUNDT=SOURCE(F)    ! LINK IN TREE         
         FOUND=TREE(FOUNDT,2)! LINK IN CNET
C---REMOVE THIS LINK FROM SOURCE
         ELEV=COORD(CNET(FOUND,2),5)
         CALL REMOVE(SOURCE,F,NS)
		if(err.ne.0) then
			close(12)
			return
		endif
C---IF THERE ARE LINKS UPSTREAM OF F ADD THEM TO TREE AND SOURCE SET
         IF(CNET(FOUND,5).GT.0)THEN
          LINK1=CNET(FOUND,5)
          ICRD1=MAX(CNET(LINK1,3)-1,CNET(LINK1,2))
          AREA1=COORD(ICRD1,5)
          LINK2=CNET(FOUND,6)
          ICRD2=MAX(CNET(LINK2,3)-1,CNET(LINK2,2))
          AREA2=COORD(ICRD2,5)
          IF(AREA1.GE.AMIN.AND.AREA2.GE.AMIN)THEN
C---ADD LINKS UPSTREAM OF FOUND TO TREE
           IT=IT+1
           if(it .gt. nl)stop 'IT overflowed tree - logic error'
           CALL ADD(SOURCE,IT,NS)
           TREE(IT,1)=CNET(FOUND,5)
           TREE(IT,2)=CNET(FOUND,5)
           TREE(IT,3)=FOUNDT
           TREE(FOUNDT,4)=IT
           IT=IT+1
           if(it .gt. nl)stop 'IT overflowed tree - logic error'
           CALL ADD(SOURCE,IT,NS)
           TREE(IT,1)=CNET(FOUND,6)
           TREE(IT,2)=CNET(FOUND,6)
           TREE(IT,3)=FOUNDT
           TREE(FOUNDT,5)=IT
           N=N+1
           if(n .gt. nmax)stop 'N overflowed nmax - logic error'    
          ELSE IF(AREA1.GE.AMIN)THEN
           TREE(FOUNDT,2)=LINK1
           CALL ADD(SOURCE,FOUNDT,NS)
          ELSE IF(AREA2.GE.AMIN)THEN
           TREE(FOUNDT,2)=LINK2
           CALL ADD(SOURCE,FOUNDT,NS)
          ENDIF
         ENDIF
       ENDDO          
           CALL ANAL(TREE,IT,COORD,CNET,N,ISUP,AMIN)
cc       WRITE(6,*)'FINAL MAGNITUDE =',N
       END   
C
C      SUBROUTINE TO ANALYSE TREE STRUCTURE
C                                           
      SUBROUTINE ANAL(TREE,IT,COORD,CNET,N,ISUP,AMIN)
      parameter(nlmax=50000,nc=500000) 
      INTEGER CNET(0:nlmax,7),TREE(nlmax,5),GEN(nlmax),MAG(nlmax),
     &IORD(nlmax),links(nlmax),linke(nlmax),iarc(nlmax)
      REAL COORD(0:nc,5),HEIGHT(nlmax),DELEV,LENGTH(nlmax),
     &GLEN(nlmax),DAREA(nlmax),xout(nc)                               
      LOGICAL DONE
      character*240 outbuf
C    
C    SCAN THROUGH TREE COMPUTING GENERATION, HEIGHT, LENGTH,
C     GEOMETRIC LENGTH, DOWNSTREAM ELEVATION, AND MAGNITUDE OF EACH LINK
C
C---COMPUTE MAGNITUDE AND ORDER
C
C----INITIALISE
      DO 3 I=1,IT
        IORD(I)=0
 3      MAG(I)=0
 1    DONE=.TRUE.
      DO 4 I=1,IT 
       IF(MAG(I).EQ.0)THEN
        IUP1=TREE(I,4)
        IUP2=TREE(I,5)
        IF(IUP1.EQ.0.AND.IUP2.EQ.0)THEN
          MAG(I)=1
          IORD(I)=1
        ELSE IF(MAG(IUP1).GT.0.AND.MAG(IUP2).GT.0)THEN
          MAG(I)=MAG(IUP1)+MAG(IUP2)
          IOMAX=MAX(IORD(IUP1),IORD(IUP2))
          IOMIN=MIN(IORD(IUP1),IORD(IUP2))
          IORD(I)=MAX(IOMAX,IOMIN+1)
        ELSE    ! PASS FOR NOW              
          DONE=.FALSE.
        ENDIF
       ENDIF
 4    CONTINUE        
      IF(.NOT.DONE)GO TO 1
C--WRITE HEADERS
      write(12,100)
  100 format("EXP  0  *.e00 file created by arclinks"/"ARC  2")      
c
c   scan through tree
c
      iarcs=0
      DO 2 I=1,IT
C---GENERATIONS
        IF(I.EQ.1)THEN
          GEN(I)=0
        ELSE
          GEN(I)=GEN(TREE(I,3))+1
        ENDIF  
C---HEIGHTS
        LINKE(i)=TREE(I,1)   ! END LINK NO IN CNET
        LINKS(i)=TREE(I,2)   ! START LINK NO IN CNET
        IUP=CNET(LINKS(i),2) ! UPSTREAM POINT IN COORD
C---SEARCH DOWN LINK FOR AREA .GE. AMIN
 10     IF(COORD(IUP,5).LT.AMIN)THEN
          IUP=IUP+1
          GO TO 10
        ENDIF
        ID=CNET(LINKE(i),3)  ! DOWNSTREAM PT IN COORD
        HEIGHT(i)=COORD(IUP,4)-COORD(ID,4)
        LENGTH(i)=COORD(IUP,3)-COORD(ID,3)
        DX=COORD(IUP,1)-COORD(ID,1)
        DY=COORD(IUP,2)-COORD(ID,2)
        GLEN(i)=SQRT(DX*DX+DY*DY)
        DELEV=COORD(ID,4)
C---OBTAIN AREA FROM PIXEL JUST BEFORE END OF LINK SINCE PIXEL AT END OF
C    LINK ALSO CONTAINS AREA OF MERGING PIXELS
C
        IARP=MAX(IUP,ID-1)     
        DAREA(i)=COORD(IARP,5)   
c        SLOPE=HEIGHT/MAX(LENGTH,.0001)
c        RNO=LINKS+LINKE/100000.
c       WRITE(12,100)RNO,GEN(I),HEIGHT,LENGTH,GLEN,DELEV
c    &  ,IORD(I),MAG(I),DAREA,SLOPE
c 100    FORMAT(1X,F12.5,I6,4F9.2,2I6,2E12.5)
c  Load coords into temporary buffer array
        nout=0
        i1=links(i) 
 46     do 47 m=iup,cnet(i1,3)
          nout=nout+1
          xout(nout)=coord(m,1)
          nout=nout+1
          xout(nout)=coord(m,2)
 47     continue
        if(i1 .ne. linke(i))then
          i1 = cnet(i1,4)   ! next link down
          iup = cnet(i1,2)+1  ! dont repeat first pt
          go to 46
        endif
c        
C     WRITE OUT THE COORDINATES OF THE CONTIGUOUS STREAM
C
        np=nout/2  
        do ip = 1,np,499
c         by experimentation 500 (and confirmed in ARC documentation)
c         is the limit that seems to work so break into multiple
c         pseudo arc's.
          i1=(ip-1)*2+1
          i2=min(i1+999,nout)
          if(i2-i1 .gt. 2 .or. ip .eq. 1)then ! dont write out single 
C           points at end of arc's, but include single point links.
            IARCs = IARCs + 1
            if(iarcs .gt. nlmax)stop 'Too many arcs on output'
            iarc(iarcs)=i
            write(12,200) iarcs,i,0,0,0,0,(i2-i1+1)/2
 200        format(7I10)
            write(12,250)(xout(m),m=i1,i2)
 250        format(4e14.7) 
          endif
        enddo
 2    CONTINUE
c
c -- write the rest of the file
      write(12,200)-1,0,0,0,0,0,0
      write(12,300)
 300  format(
     &"TOL  2"/
     &"         1         2 2.4000000E-02"/
     &"         2         2 0.0000000E+00"/
     &"         3         2 0.0000000E+00"/
     &"         4         2 0.0000000E+00"/
     &"         5         2 0.0000000E+00"/
     &"         6         2 2.4000000E+00"/
     &"         7         2 2.4000000E-01"/
     &"         8         2 2.4000000E-01"/
     &"         9         2 2.4000000E-01"/
     &"        10         2 2.4000000E-01")
      write(12,200)-1,0,0,0,0,0,0
      write(12,401)iarcs
c234567890123456789012345678901234567890123456789012345678901234567890
 401  format(
     &"IFO  2"/
     &"generic.AAT                     XX  15  15  32",i10/
     &"FNODE#            4-1   14-1   5-1 50-1  -1  -1-1",
     &"                   1-"/
     &"TNODE#            4-1   54-1   5-1 50-1  -1  -1-1",
     &"                   2-"/
     &"LPOLY#            4-1   94-1   5-1 50-1  -1  -1-1",
     &"                   3-"/
     &"RPOLY#            4-1  134-1   5-1 50-1  -1  -1-1",
     &"                   4-"/
     &"link.Length(m)    4-1  174-1  12 2 60-1  -1  -1-1",
     &"                   5-"/
     &"generic#          4-1  214-1   5-1 50-1  -1  -1-1",
     &"                   6-"/
     &"generic-ID        4-1  254-1   5-1 50-1  -1  -1-1",
     &"                   7-"/
     &"LINKS             4-1  294-1   5-1 50-1  -1  -1-1",
     &"                   8-"/
     &"LINKE             4-1  334-1   5-1 50-1  -1  -1-1",
     &"                   9-"/
     &"Drop(m)           4-1  374-1  12 2 60-1  -1  -1-1",
     &"                  10-"/
     &"Geom.Length(m)    4-1  414-1  12 2 60-1  -1  -1-1",
     &"                  11-"/
     &"ORDER             4-1  454-1   5-1 50-1  -1  -1-1",
     &"                  12-"/
     &"Magnitude         4-1  494-1   5-1 50-1  -1  -1-1",
     &"                  13-"/
     &"Area(m**2)        4-1  534-1  12 0 60-1  -1  -1-1",
     &"                  14-"/
     &"Slope             4-1  574-1  12 5 60-1  -1  -1-1",
     &"                  15-")

      do 9 i=1,iarcs
        slope=HEIGHT(iarc(i))/MAX(LENGTH(iarc(i)),.0001)
        write(outbuf,500)0,0,0,0,length(iarc(i)),i,iarc(i),
     +  links(iarc(i)),
     +  linke(iarc(i)),
     +  height(iarc(i)),
     &  glen(iarc(i)),iord(iarc(i)),mag(iarc(i)),darea(iarc(i)),slope
 500    format(4I11,e14.7,2i11,2I11,2e14.7,2i11,2e14.7)
        write(12,501)outbuf(1:80)
        write(12,501)outbuf(81:160)
        write(12,501)outbuf(161:240)
 501    format(a80)
c       write(12,500)0,0,0,0,links(iarc(i)),linke(iarc(i)),
c    +  height(iarc(i)),length(iarc(i)),
c    &  glen(iarc(i)),iord(iarc(i)),mag(iarc(i)),darea(iarc(i)),slope
c500    format(6I11,e14.7/2e14.7,2i11,2e14.7)

 9    continue

      WRITE(12,600) 
 600  format("EOI"/"EOS")
      CLOSE(12)
      CLOSE(13) 
 
      RETURN                         
      END              
C
C      PROGRAM TO EXTRACT STRAHLER STREAMS OF CHANNEL NETWORK TREE STRUCTURE
C       CREATED BY DAVID G TARBOTON
C       THIS VERSION EXTRACTS NETWORK BASED ON MIN SUPPORT AREA
C       and outputs in ARC e00 format
c       2-11-98
C
      subroutine ARCSTREAMS(coordfile,treefile,outfile,ilink,amin,err)
	!DEC$ ATTRIBUTES DLLEXPORT::ARCSTREAMS
	 parameter(nlmax=50000,nc=500000)   !  remember to change subroutines
       INTEGER CNET(0:nlmax,7),TREE(nlmax,5),SOURCE(nlmax/2+1),
     & F,FOUND,FOUNDT,err       
       REAL COORD(0:nc,5)
       CHARACTER*4096 COORDFILE,TREEFILE,OUTFILE
C  
C     DATA STRUCTURE:
C     ---------------
C     COORD CONTAINS DIGITISED POINTS AS FOLLOWS:
C     COORD(*,1) = X COORDINATE (metres)
C       "    ,2    Y    "
C       "    ,3    DISTANCE ALONG CHANNELS TO GAUGE (metres)
C       "    ,4    ELEVATION
C       "    ,5    area M**2
C                                                
C     CNET CONTAINS LINKS AS STRINGS OF DIGITISED POINTS AS FOLLOWS:
C     CNET(*,1) = LINK NUMBER                                       
C       "   ,2    START POINT NUMBER IN COORD
C       "   ,3    END POINT NUMBER IN COORD
C       "   ,4    NEXT (DOWNSTREAM) LINK NUMBER IN CNET
C       "   ,5&6  PREVIOUS (UPSTREAM) LINK NUMBERS IN CNET
C       "   ,7    STRAHLER ORDER OF LINK IN CNET
C
C     TREE CONTAINS THE NETWORK AS BUILT UP DURING EXECUTION
C     TREE(*,1) = LAST LINK NUMBER (POINTS TO CNET)
C     TREE(*,2) = FIRST LINK NUMBER (POINTS TO CNET)
C       "   ,3    NEXT (DOWNSTREAM) LINK (POINTS TO TREE)
C       "   ,4&5  PREVIOUS (U.S.) LINKS (POINTS TO TREE) (0 WHEN NOT FULL)
C
C     CNET contains one more line than TREE.  This is necessary due 
C     to the end = option in the read.
C
C     SOURCE CONTAINS LINK NUMBERS (POINTS TO TREE) OF FIRST ORDER STREAMS
C      STILL OPEN I.E. MAY BIFURCATE UPSTREAM AT HIGHER ELEVATION
C 
c       OPEN(UNIT=9,FILE='netex.in',STATUS='OLD')
c       READ(9,10)TREEFILE,COORDFILE,OUTFILE
c 10    FORMAT(///////A80//A80/A80)
         OPEN(UNIT=11,FILE=TREEFILE,STATUS='OLD')
         OPEN(UNIT=12,FILE=OUTFILE,STATUS='unknown')
	err = 0
C---READ IN LINKS
       DO NL=0,nlmax
         READ(11,*,END=333)(CNET(NL,I),I=1,7)
       ENDDO
c --- upon exiting this loop, nl contains the number of links read
	err = 1
cc       WRITE(6,*)'TOO MANY LINKS'     
 333   close(11)
         OPEN(UNIT=10,FILE=COORDFILE,STATUS='OLD')
C---READ IN COORDS AND ELEVATIONS
       DO I=0,nc
         READ(10,*,END=444)(COORD(I,J),J=1,5)
       ENDDO
	err = 2
cc       WRITE(6,*)'TOO MANY CO ORDS'
C--- GET MAX MAGNITUDE AND START LINK
 444   CONTINUE                   
       close(10)
	if(err.ne.0) return
C       WRITE(6,*)'INPUT MAGNITUDE AND START LINK'
c       read(9,*)   ! skip lines
c       read(9,*)
c       READ(9,*)NMAX,ILINK,ISUP,APIX
       nmax = (nl-1)/2+1
c       READ(9,*)ILINK,ISUP,APIX
c       AMIN=ISUP*APIX
C                                         
C---INITIALISE        
       N=1
       NS=1             ! SOURCE SET POINTER
       SOURCE(1)=1      ! SOURCE SET INITIALLY ROOT LINK
       IT=1             ! TREE POINTER
       DO J=1,nl          ! INITIALISE TREE ARRAY TO ZERO
         DO K=1,5
           TREE(J,K)=0
         ENDDO
       ENDDO
       TREE(1,1)=ILINK           ! TREE INITIALLY ROOT LINK STARTS FROM
       TREE(1,2)=ILINK           ! Inserted DGT 8/13/93
C                                  POSITION 1, VALUE ZERO INDICATES NO BRANCH
       DO WHILE (NS.GT.0)    ! SOURCE SET NOT EMPTY 
C---SEARCH SOURCE SET FOR LINK WITH highest UPSTREAM area
         F=1
         DO J=2,NS
           IF(COORD(CNET(TREE(SOURCE(J),2),2),5).GT.
     &     COORD(CNET(TREE(SOURCE(F),2),2),5))F=J
         ENDDO                 
         FOUNDT=SOURCE(F)    ! LINK IN TREE         
         FOUND=TREE(FOUNDT,2)! LINK IN CNET
C---REMOVE THIS LINK FROM SOURCE
         ELEV=COORD(CNET(FOUND,2),5)
         CALL REMOVE(SOURCE,F,NS)
	   IF(err.ne.0) then
			close(12)
			return
	   endif
C---IF THERE ARE LINKS UPSTREAM OF F ADD THEM TO TREE AND SOURCE SET
         IF(CNET(FOUND,5).GT.0)THEN
          LINK1=CNET(FOUND,5)
          ICRD1=MAX(CNET(LINK1,3)-1,CNET(LINK1,2))
          AREA1=COORD(ICRD1,5)
          LINK2=CNET(FOUND,6)
          ICRD2=MAX(CNET(LINK2,3)-1,CNET(LINK2,2))
          AREA2=COORD(ICRD2,5)
          IF(AREA1.GE.AMIN.AND.AREA2.GE.AMIN)THEN
C---ADD LINKS UPSTREAM OF FOUND TO TREE
           IT=IT+1
           if(it .gt. nl) then
			!stop 'IT overflowed tree - logic error'
			err=6
			RETURN
		 end if
		 CALL ADD(SOURCE,IT,NS)
           TREE(IT,1)=CNET(FOUND,5)
           TREE(IT,2)=CNET(FOUND,5)
           TREE(IT,3)=FOUNDT
           TREE(FOUNDT,4)=IT
           IT=IT+1
           if(it .gt. nl) then
			!stop 'IT overflowed tree - logic error'
			err=6
			close(12)
			RETURN
		 end if
           CALL ADD(SOURCE,IT,NS)
           TREE(IT,1)=CNET(FOUND,6)
           TREE(IT,2)=CNET(FOUND,6)
           TREE(IT,3)=FOUNDT
           TREE(FOUNDT,5)=IT
           N=N+1
           if(n .gt. nmax) then
			!stop 'N overflowed nmax - logic error'  
			err=7
			close(12)
			RETURN
		 end if 
          ELSE IF(AREA1.GE.AMIN)THEN
           TREE(FOUNDT,2)=LINK1
           CALL ADD(SOURCE,FOUNDT,NS)
          ELSE IF(AREA2.GE.AMIN)THEN
           TREE(FOUNDT,2)=LINK2
           CALL ADD(SOURCE,FOUNDT,NS)
          ENDIF
         ENDIF
       ENDDO          
c---write headers
c       write(12,277)ISUP,AMIN,treefile
C 277   format(1x,I5,E12.4,' streams from ',a80/1x,'7'/1x,'first link'/
C     & 1x,'last link'/1x,'order'/1x,'length'/1x,'drop'/1x,'g. length'/
C     & 1X,'AREA')
           CALL ANAL2(TREE,IT,COORD,CNET,N,AMIN)                       
C       WRITE(6,*)'FINAL MAGNITUDE =',N
       END   
C            
C      SUBROUTINE TO ANALYSE TREE STRUCTURE
C
      SUBROUTINE ANAL2(TREE,IT,COORD,CNET,N,AMIN)
      parameter(nlmax=50000,nc=500000)   
      INTEGER CNET(0:nlmax,7),TREE(nlmax,5),IORD(nlmax)
      INTEGER sord(nlmax),iarc(nlmax)
      REAL COORD(0:nc,5),xout(nc),slen(nlmax) 
      LOGICAL IDONE(nlmax),DONE
C
C---COMPUTE  ORDER
C
C----INITIALISE
      DO 3 I=1,IT
        IORD(I)=0
 3    CONTINUE
 1    DONE=.TRUE.
      DO 4 I=1,IT 
       IF(IORD(I).EQ.0)THEN
        IUP1=TREE(I,4)
        IUP2=TREE(I,5)
        IF(IUP1.EQ.0.AND.IUP2.EQ.0)THEN
          IORD(I)=1
        ELSE IF(IORD(IUP1).GT.0.AND.IORD(IUP2).GT.0)THEN
          IOMAX=MAX(IORD(IUP1),IORD(IUP2))
          IOMIN=MIN(IORD(IUP1),IORD(IUP2))
          IORD(I)=MAX(IOMAX,IOMIN+1)
        ELSE    ! PASS FOR NOW              
          DONE=.FALSE.
        ENDIF
       ENDIF
 4    CONTINUE        
      IF(.NOT.DONE)GO TO 1
C----INITIALISE DONE FLAG
      DO 5 I=1,IT                         
 5      IDONE(I)=.FALSE.
      iarcs = 0
      iarcn=0
c---Initialize output
      write(12,100)
  100 format("EXP  0  *.e00 file created by arcstreams"/"ARC  2")      
C    
C    SCAN THROUGH TREE writing 
C     STRAHLER STREAMS
C     

      DO 2 I=1,IT
        IF(IDONE(I))GO TO 2
        JORD=IORD(I)        
        IF(JORD.EQ.1)THEN
          IFIRST=TREE(I,2)
          ILAST=TREE(I,1)
          IDONE(I)=.TRUE.
        ELSE             
C---SCAN DOWNSTREAM FOR END OF STREAM
          IC=I
 6        NEXTL=TREE(IC,3)
          IF(NEXTL.EQ.0)THEN
            ILAST=TREE(IC,1)
          ELSE IF(IORD(NEXTL).EQ.JORD)THEN
            IC=NEXTL
            IDONE(IC)=.TRUE.
            GO TO 6
          ELSE    ! HAVE REACHED END OF STREAM
            ILAST=TREE(IC,1)
          ENDIF
C---SCAN UPSTREAM FOR START
          IC=I
 7        IP1=TREE(IC,4)                  
          IP2=TREE(IC,5)
          IF(IORD(IP1).EQ.JORD)THEN
            IC=IP1
            IDONE(IC)=.TRUE.
            GO TO 7
          ELSE IF(IORD(IP2).EQ.JORD)THEN
            IC=IP2
            IDONE(IC)=.TRUE.
            GO TO 7
          ELSE  ! HAVE REACHED START OF STREAM
            IFIRST=TREE(IC,2)
          ENDIF
        ENDIF                     
        icrd1=cnet(ifirst,2)
c---SEARCH DOWN LINK FOR AREA .GE.AMIN
 10     IF(COORD(ICRD1,5).LT.AMIN)THEN
          ICRD1=ICRD1+1
          GO TO 10
        ENDIF         
C---CHECK THAT DID NOT OVERFLOW TOP LINK
        IF(ICRD1.GT.CNET(IFIRST,3))err=3 !WRITE(6,*)'LOGIC ERROR'
        XLEN=COORD(ICRD1,3)-COORD(CNET(ILAST,3),3)
        DROP=COORD(ICRD1,4)-COORD(CNET(ILAST,3),4)
        G1=COORD(ICRD1,1)-COORD(CNET(ILAST,3),1)
        G2=COORD(ICRD1,2)-COORD(CNET(ILAST,3),2)
        GL=SQRT(G1*G1+G2*G2)
        IUP=CNET(ILAST,2)    ! UPSTREAM PT IN LAST LINK
        ID=CNET(ILAST,3)     ! DOWNSTREAM PT IN LAST LINK
C---OBTAIN AREA FROM PIXEL JUST BEFORE END OF STREAM SINCE PIXEL AT END OF
C    LINK ALSO CONTAINS AREA OF MERGING PIXELS
C
        IARP=MAX(IUP,ID-1)     
        DAREA=COORD(IARP,5)   
 
c  Load coords into temporary buffer array
        nout=0 
 46     do 47 m=icrd1,cnet(ifirst,3)
          nout=nout+1
          if(nout .gt. nc) then
		   !stop 'Error - xout dimensions too small'
			err=5
			close(12)
			RETURN
		end if
          xout(nout)=coord(m,1)
          nout=nout+1
          if(nout .gt. nc) then
			!stop 'Error - xout dimensions too small'
			err = 5
			close(12)
			RETURN
		end if
		xout(nout)=coord(m,2)
 47     continue
        if(ifirst .ne. ilast)then
          ifirst = cnet(ifirst,4)   ! next link down
          icrd1 = cnet(ifirst,2)+1  ! dont repeat first pt
          go to 46
        endif
c        
C     WRITE OUT THE COORDINATES OF THE CONTIGUOUS STREAM
C
        IARCn = IARCn + 1   !  count of actual arc's
        np=nout/2  
        do ip = 1,np-1,499
c         by experimentation 500 (and confirmed in ARC documentation)
c         is the limit that seems to work so break into multiple
c         pseudo arc's.
          i1=(ip-1)*2+1
          i2=min(i1+999,nout)
          if(i2-i1 .gt. 2)then ! dont write out single points
            IARCs = IARCs + 1
            iarc(iarcs)=iarcn
            write(12,200) iarcs,iarcn,0,0,0,0,(i2-i1+1)/2
 200        format(7I10)
            write(12,250)(xout(m),m=i1,i2)
 250        format(4e14.7) 
          endif
        enddo
      sord(IARCn)=jord
      slen(IARCn)=xlen 
c     save attributes for later output
 2    CONTINUE   
c
c -- write the rest of the file
      write(12,200)-1,0,0,0,0,0,0
      write(12,300)
 300  format(
     &"TOL  2"/
     &"         1         2 2.4000000E-02"/
     &"         2         2 0.0000000E+00"/
     &"         3         2 0.0000000E+00"/
     &"         4         2 0.0000000E+00"/
     &"         5         2 0.0000000E+00"/
     &"         6         2 2.4000000E+00"/
     &"         7         2 2.4000000E-01"/
     &"         8         2 2.4000000E-01"/
     &"         9         2 2.4000000E-01"/
     &"        10         2 2.4000000E-01")
      write(12,200)-1,0,0,0,0,0,0
      write(12,400)iarcs
c234567890123456789012345678901234567890123456789012345678901234567890
 400  format(
     &"IFO  2"/
     &"SM1.AAT                         XX   8   8  32",i10/
     &"FNODE#            4-1   14-1   5-1 50-1  -1  -1-1",
     &"                   1-"/
     &"TNODE#            4-1   54-1   5-1 50-1  -1  -1-1",
     &"                   2-"/
     &"LPOLY#            4-1   94-1   5-1 50-1  -1  -1-1",
     &"                   3-"/
     &"RPOLY#            4-1  134-1   5-1 50-1  -1  -1-1",
     &"                   4-"/
     &"SLENGTH           4-1  174-1  12 3 60-1  -1  -1-1",
     &"                   5-"/
     &"SM1#              4-1  214-1   5-1 50-1  -1  -1-1",
     &"                   6-"/
     &"SM1-ID            4-1  254-1   5-1 50-1  -1  -1-1",
     &"                   7-"/
     &"ORDER             4-1  294-1   5-1 50-1  -1  -1-1",
     &"                   8-")
      do 9 m = 1,iarcs
        write(12,500)0,0,0,0,slen(iarc(m)),m,iarc(m),sord(iarc(m))
 500    format(4I11,e14.7,2i11/i11)
 9    continue

      WRITE(12,600) 
 600  format("EOI"/"EOS")
      CLOSE(12)
      CLOSE(13)
      RETURN
      END              
C
C      ROUTINE TO REMOVE ELEMENT FROM SOURCE SET
C             
         SUBROUTINE REMOVE(SOURCE,F,NS)
         INTEGER SOURCE(1),F
         IF(F.LT.NS)THEN
           SOURCE(F)=SOURCE(NS)
           NS=NS-1        
         ELSE IF(F.EQ.NS)THEN
           NS=NS-1
         ELSE
		 err = 4
cc           WRITE(6,*)' REMOVE ERROR NS=',NS,'  J=',J
         ENDIF
         RETURN
         END
C
C     SUBROUTINE TO ADD ELEMENT TO SOURCE SET
C
         SUBROUTINE ADD(SOURCE,IVAL,NS)
         INTEGER SOURCE(1)
         NS=NS+1
         SOURCE(NS)=IVAL
         RETURN
         END
       
