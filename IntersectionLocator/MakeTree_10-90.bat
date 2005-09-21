@echo off

rem This bat file creates a directory tree for creating output from
rem the intersection locator's current driver.  It is really just to
rem save me time more than anything.  Note that the clean option is
rem very dangerous.  Make sure you are doing what you intend to.
rem
rem Programmer(If it can be called that?): Ryan Stelzleni
rem Date: 10/18/04

rem Some kind of CVS incantation here
rem $Id: MakeTree_10-90.bat,v 1.1 2005/09/21 15:27:20 ahartman Exp $



if %1 == %nothing% goto end
if %1 == clean goto clean

mkdir %1\Unfiltered
mkdir %1\Filter_10DF
mkdir %1\Filter_10VMF
mkdir %1\Filter_20DF
mkdir %1\Filter_20VMF
mkdir %1\Filter_30DF
mkdir %1\Filter_30VMF
mkdir %1\Filter_40DF
mkdir %1\Filter_40VMF
mkdir %1\Filter_50DF
mkdir %1\Filter_50VMF
mkdir %1\Filter_60DF
mkdir %1\Filter_60VMF
mkdir %1\Filter_70DF
mkdir %1\Filter_70VMF
mkdir %1\Filter_80DF
mkdir %1\Filter_80VMF
mkdir %1\Filter_90DF
mkdir %1\Filter_90VMF
mkdir %1\Unfiltered\Points
mkdir %1\Filter_10DF\Points
mkdir %1\Filter_10VMF\Points
mkdir %1\Filter_20DF\Points
mkdir %1\Filter_20VMF\Points
mkdir %1\Filter_30DF\Points
mkdir %1\Filter_30VMF\Points
mkdir %1\Filter_40DF\Points
mkdir %1\Filter_40VMF\Points
mkdir %1\Filter_50DF\Points
mkdir %1\Filter_50VMF\Points
mkdir %1\Filter_60DF\Points
mkdir %1\Filter_60VMF\Points
mkdir %1\Filter_70DF\Points
mkdir %1\Filter_70VMF\Points
mkdir %1\Filter_80DF\Points
mkdir %1\Filter_80VMF\Points
mkdir %1\Filter_90DF\Points
mkdir %1\Filter_90VMF\Points
mkdir %1\Unfiltered\Lines
mkdir %1\Filter_10DF\Lines
mkdir %1\Filter_10VMF\Lines
mkdir %1\Filter_20DF\Lines
mkdir %1\Filter_20VMF\Lines
mkdir %1\Filter_30DF\Lines
mkdir %1\Filter_30VMF\Lines
mkdir %1\Filter_40DF\Lines
mkdir %1\Filter_40VMF\Lines
mkdir %1\Filter_50DF\Lines
mkdir %1\Filter_50VMF\Lines
mkdir %1\Filter_60DF\Lines
mkdir %1\Filter_60VMF\Lines
mkdir %1\Filter_70DF\Lines
mkdir %1\Filter_70VMF\Lines
mkdir %1\Filter_80DF\Lines
mkdir %1\Filter_80VMF\Lines
mkdir %1\Filter_90DF\Lines
mkdir %1\Filter_90VMF\Lines
goto end

:clean
rmdir /S /Q %2

:end

echo.Done

