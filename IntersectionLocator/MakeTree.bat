@echo off

rem This bat file creates a directory tree for creating output from
rem the intersection locator's current driver.  It is really just to
rem save me time more than anything.  Note that the clean option is
rem very dangerous.  Make sure you are doing what you intend to.
rem
rem Programmer(If it can be called that?): Ryan Stelzleni
rem Date: 10/18/04

rem Some kind of CVS incantation here
rem $Id: MakeTree.bat,v 1.2 2004/10/18 22:56:07 rstelzleni Exp $



if %1 == %nothing% goto end
if %1 == clean goto clean

mkdir %1\DiffUnfiltered
mkdir %1\Diff_10DF
mkdir %1\Diff_10VMF
mkdir %1\Diff_20DF
mkdir %1\Diff_20VMF
mkdir %1\Diff_30DF
mkdir %1\Diff_30VMF
mkdir %1\Diff_40DF
mkdir %1\Diff_40VMF
mkdir %1\Diff_50DF
mkdir %1\Diff_50VMF
goto end

:clean
rmdir /S /Q %2

:end

echo.Done

