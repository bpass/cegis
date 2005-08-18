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
C
C      PROGRAM TO EXTRACT STRAHLER STREAMS OF CHANNEL NETWORK TREE STRUCTURE
C       CREATED BY DAVID G TARBOTON
C       THIS VERSION EXTRACTS NETWORK BASED ON MIN SUPPORT AREA
C
C
       subroutine STREAMAN(coordfile,treefile,outfile,ilink,amin,err)
	!DEC$ ATTRIBUTES DLLEXPORT::STREAMAN
       parameter(nlmax=50000, nc=500000)   !   Remember to change in subroutines
       INTEGER CNET(0:nlmax,7),TREE(nlmax,5),SOURCE(nlmax),
     & E(nlmax),F,FOUND,FOUNDT,err
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
C     E CONTAINS ELEVATIONS CORRESPONDING TO MAGNITUDE (ELEVATION AT WHICH
C       BIFURCATION TO THAT MAGNITUDE OCCURS WHEN WORKING UPWARDS)
C
c       OPEN(UNIT=9,FILE='netex.in',STATUS='OLD')
c       READ(9,10)TREEFILE,COORDFILE,OUTFILE
c 10    FORMAT(///////A80//A80/A80)
         OPEN(UNIT=11,FILE=TREEFILE,STATUS='OLD')
         OPEN(UNIT=12,FILE=OUTFILE,STATUS='unknown')
C---READ IN LINKS
       DO N=0,nlmax
         READ(11,*,END=333)(CNET(N,I),I=1,7)
       ENDDO
	 err=1
cc       WRITE(6,*)'TOO MANY LINKS'
 333   NL=N-1     
       close(11)
         OPEN(UNIT=10,FILE=COORDFILE,STATUS='OLD')
C---READ IN COORDS AND ELEVATIONS
       DO I=0,nc
         READ(10,*,END=444)(COORD(I,J),J=1,5)
       ENDDO
	 err=2
cc       WRITE(6,*)'TOO MANY CO ORDS'
C--- GET MAX MAGNITUDE AND START LINK
 444   CONTINUE                   
       close(10)
	 if(err.ne.0) then
		close(12)
		return
	 endif
C       WRITE(6,*)'INPUT MAGNITUDE AND START LINK'
c      read(9,*)   ! skip lines
c      read(9,*)
c       READ(9,*)NMAX,ILINK,ISUP,APIX
       nmax = nlmax/2-1
c       READ(9,*)ILINK,ISUP,APIX
c       AMIN=ISUP*APIX
C                                         
C---INITIALISE        
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
       TREE(1,2)=ILINK           ! Inserted DGT 8/13/93
C                                  POSITION 1, VALUE ZERO INDICATES NO BRANCH
       E(N)=COORD(CNET(TREE(1,1),3),4)
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
         ELEV=COORD(CNET(FOUND,2),5)
         CALL REMOVE(SOURCE,F,NS)
	   if(err.ne.0)then
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
           E(N)=ELEV    
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
       write(12,277)AMIN
 277   format(1x,'Streams with amin = ',E12.5/1x,'7'/1x,'first link'/
     & 1x,'last link'/1x,'order'/1x,'length'/1x,'drop'/1x,'g. length'/
     & 1X,'AREA')
           CALL ANAL3(TREE,IT,COORD,CNET,N,AMIN)                       
cc       WRITE(6,*)'FINAL MAGNITUDE =',N
		close(12)
       END   
C            
C      SUBROUTINE TO ANALYSE TREE STRUCTURE
C
      SUBROUTINE ANAL3(TREE,IT,COORD,CNET,N,AMIN)
      parameter(nlmax=50000, nc=500000)
      INTEGER CNET(0:nlmax,7),TREE(nlmax,5),IORD(nlmax)
      REAL COORD(0:nc,5)                          
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
C    
C    SCAN THROUGH TREE RECORDING START AND END LINK OF 
C     STRAHLER STREAMS
C     
C----INITIALISE DONE FLAG
      DO 5 I=1,IT                         
 5      IDONE(I)=.FALSE.
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
        IF(ICRD1.GT.CNET(IFIRST,3)) then
	  !WRITE(6,*)'LOGIC ERROR'
		err=3
		return
	  endif
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
      WRITE(12,100)IFIRST,ILAST,JORD,XLEN,DROP,GL,DAREA
 100  FORMAT(1X,3I6,4E13.5)
 2    CONTINUE
      RETURN
	END