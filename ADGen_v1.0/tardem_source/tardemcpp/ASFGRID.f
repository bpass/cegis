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
c
c      program to determine slope vs area function for dem
c
      subroutine ASFGRID(area,slope,a,s,nclass,method,outfile,sortfile,
     & nx,ny,dx,dy,err)
	!DEC$ ATTRIBUTES DLLEXPORT::ASFGRID
c	PARAMETER(IGRIdx=3701,igridy=3701)
      DIMENSION S(nx*ny),
     &A(nx*ny)
c	  parameter(ishrdt = 1, iintdt = 2, ifltdt=3)
c	  double precision BNDBOX(4),csize
      dimension AREA(ny,nx)
      dimension slope(ny,nx)
c      Equivalence (a(1),area(1,1))
      CHARACTER*4096 OUTFILE,sortfile
	INTEGER err
      NL=nx*ny
C
C    READ INPUT
C
c       OPEN(UNIT=11,FILE='asf.in',STATUS='OLD')
c       READ(11,22)slopeFILE,AREAFILE,outFILE,sortFILE
c 22    FORMAT(A80/a80/a80/a80)
c       write(6,*)"Input bin class size"
c       read(11,*)NCLASS
c       CALL demread(slope,slopeFILE,NX,NY,IGRIDY,DX,DY)
c       call fgridread(slopefile,slope,ifltdt,nx,ny,dx,dy,
c     &  bndbox,csize,xndv,iftype,igridy)
c       CALL demread(area,areaFILE,NXN,NYN,IGRIDY,DX,DY)
c       call fgridread(areafile,area,ifltdt,nxn,nyn,dx,dy,
c     &  bndbox,csize,xndv,iftype,igridy)

c       IF(NXN.NE.NX.OR.NYN.NE.NY)WRITE(6,*)'INCOMPATIBLE file SIZES'
	apix = dx
      if(method .eq. 2)APIX= DX*DY   
      N=0
      DO 3 J=2,NX-1        !  Process in column order so that displaced area
        DO 3 I=2,NY-1      !   is never used. Save memory.
c          write(6,*)i,j,area(i,j)
          IF(AREA(I,J).GT.0.)THEN
            N=N+1
            A(N)=APIX*AREA(I,J)
            S(N)=SLOPE(i,j)
          ENDIF
 3    CONTINUE
      if(n .gt. 0)then
       CALL SORT2(N,A,S)
	else
	  !write(6,*)'Error no data to sort'
		err=1
	    return
	endif
       OPEN(UNIT=11,FILE=OUTFILE)
       open(unit=12,file=sortfile)
       WRITE(11,50)
 50    FORMAT(
     & 1X,'no',1X,'area',
     & 1X,'slope',1X,'svar')
       write(12,*)"area slope"
C---COMPUTE  PROPERTIES FOR EACH CLASS
       IC=1
       CMAX=A(NCLASS)
       ALSUM=0.
       SSUM=0.
       INO=0
       SSUM2=0.
       DO 5 I=1,N
         write(12,*)a(i),s(i)
         IF(A(I).LE.CMAX)THEN   ! IN CURRENT CLASS SO CONTINUE SUM
           ALSUM=ALSUM+LOG(A(I))
           SSUM=SSUM+S(I)
           SSUM2=SSUM2+S(I)**2
           INO=INO+1
         ELSE   ! WRITE OLD CLASS THEN CREATE NEW CLASS
           AMEAN=EXP(ALSUM/FLOAT(INO))
           SMEAN=SSUM/FLOAT(INO)
           varS=(Ssum2-SSUM*SMEAN)/float(max(ino-1,1))
           WRITE(11,100)INO,AMEAN,SMEAN,VARS
           INO=1
           IC=IC+1
           INDEX=MIN(I+NCLASS-1,N)
           CMAX=A(INDEX)
           ALSUM=LOG(A(I))
           SSUM=S(I)
           SSUM2=S(I)**2
         ENDIF
 5     CONTINUE
C----LAST CLASS
           AMEAN=EXP(ALSUM/FLOAT(INO))
           SMEAN=SSUM/FLOAT(INO)
           varS=(Ssum2-SSUM*SMEAN)/float(max(ino-1,1))
           WRITE(11,100)INO,AMEAN,SMEAN,VARS
 100   FORMAT(1X,I8,3E15.6)
       END
C
C     Numerical recipes Subroutine Sort
C      Uses heapsort See C Recipes p 247
C
      SUBROUTINE SORT2(N,RA,RB)
      DIMENSION RA(N),RB(N)
      L=N/2+1
      IR=N
10    CONTINUE
        IF(L.GT.1)THEN
          L=L-1
          RRA=RA(L)
          RRB=RB(L)
        ELSE
          RRA=RA(IR)
          RRB=RB(IR)
          RA(IR)=RA(1)
          RB(IR)=RB(1)
          IR=IR-1
          IF(IR.EQ.1)THEN
            RA(1)=RRA
            RB(1)=RRB
            RETURN
          ENDIF
        ENDIF
        I=L
        J=L+L
20      IF(J.LE.IR)THEN
          IF(J.LT.IR)THEN
            IF(RA(J).LT.RA(J+1))J=J+1
          ENDIF
          IF(RRA.LT.RA(J))THEN
            RA(I)=RA(J)
            RB(I)=RB(J)
            I=J
            J=J+J
          ELSE
            J=IR+1
          ENDIF
        GO TO 20
        ENDIF
        RA(I)=RRA
        RB(I)=RRB
      GO TO 10
      END
