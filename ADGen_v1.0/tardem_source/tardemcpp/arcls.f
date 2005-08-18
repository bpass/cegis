C
C      PROGRAM TO ANALYSE LINKS OF CHANNEL NETWORK TREE STRUCTURE
C       CREATED BY DAVID G TARBOTON
C       THIS VERSION EXTRACTS NETWORK BASED ON MIN SUPPORT AREA
c       and outputs in ARC e00 format
c       2-12-98
C
       subroutine arclinks(coordfile,treefile,outfile,ilink,amin)
	!MS$ATTRIBUTES DLLEXPORT :: arclinks
       parameter(nlmax=50000,nc=500000)   !  remember to change subroutines
       INTEGER CNET(0:nlmax,7),TREE(nlmax,5),SOURCE(nlmax/2+1),
     & F,FOUND,FOUNDT       
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
C---READ IN LINKS
       DO NL=0,nlmax
         READ(11,*,END=333)(CNET(NL,I),I=1,7)
       ENDDO
c --- upon exiting this loop, nl contains the number of links read
       WRITE(6,*)'TOO MANY LINKS'
 333   CLOSE(11)
         OPEN(UNIT=10,FILE=COORDFILE,STATUS='OLD',readonly)
C---READ IN COORDS AND ELEVATIONS
       DO I=0,nc
         READ(10,*,END=444)(COORD(I,J),J=1,5)
       ENDDO
       WRITE(6,*)'TOO MANY CO ORDS'
C--- GET MAX MAGNITUDE AND START LINK
 444   CONTINUE                   
       close(10)
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
       WRITE(6,*)'FINAL MAGNITUDE =',N
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
           WRITE(6,*)' REMOVE ERROR NS=',NS,'  J=',J
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

C
C      PROGRAM TO EXTRACT STRAHLER STREAMS OF CHANNEL NETWORK TREE STRUCTURE
C       CREATED BY DAVID G TARBOTON
C       THIS VERSION EXTRACTS NETWORK BASED ON MIN SUPPORT AREA
C       and outputs in ARC e00 format
c       2-11-98
C
      subroutine arcstreams(coordfile,treefile,outfile,ilink,amin)
      !MS$ATTRIBUTES DLLEXPORT :: arcstreams
	 parameter(nlmax=50000,nc=500000)   !  remember to change subroutines
       INTEGER CNET(0:nlmax,7),TREE(nlmax,5),SOURCE(nlmax/2+1),
     & F,FOUND,FOUNDT       
       REAL COORD(0:nc,5)
       CHARACTER*80 COORDFILE,TREEFILE,OUTFILE
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
C---READ IN LINKS
       DO NL=0,nlmax
         READ(11,*,END=333)(CNET(NL,I),I=1,7)
       ENDDO
c --- upon exiting this loop, nl contains the number of links read
       WRITE(6,*)'TOO MANY LINKS'     
 333   close(11)
         OPEN(UNIT=10,FILE=COORDFILE,STATUS='OLD')
C---READ IN COORDS AND ELEVATIONS
       DO I=0,nc
         READ(10,*,END=444)(COORD(I,J),J=1,5)
       ENDDO
       WRITE(6,*)'TOO MANY CO ORDS'
C--- GET MAX MAGNITUDE AND START LINK
 444   CONTINUE                   
       close(10)
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
c---write headers
c       write(12,277)ISUP,AMIN,treefile
C 277   format(1x,I5,E12.4,' streams from ',a80/1x,'7'/1x,'first link'/
C     & 1x,'last link'/1x,'order'/1x,'length'/1x,'drop'/1x,'g. length'/
C     & 1X,'AREA')
           CALL ANALs(TREE,IT,COORD,CNET,N,AMIN)                       
C       WRITE(6,*)'FINAL MAGNITUDE =',N
       END   
C            
C      SUBROUTINE TO ANALYSE TREE STRUCTURE
C
      SUBROUTINE ANALs(TREE,IT,COORD,CNET,N,AMIN)
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
        IF(ICRD1.GT.CNET(IFIRST,3))WRITE(6,*)'LOGIC ERROR'
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
          if(nout .gt. nc)stop 'Error - xout dimensions too small'
          xout(nout)=coord(m,1)
          nout=nout+1
          if(nout .gt. nc)stop 'Error - xout dimensions too small'
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

