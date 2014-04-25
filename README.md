Electrocardiagrom
=================

2011-10-20
a simple program to display data with qwt

## install [qwt-6.0][http://qwt.sourceforge.net/] ##

1. download qwt-6.0

	svn checkout svn://svn.code.sf.net/p/qwt/code/branches/qwt-6.0

2. compile && install

	qmake qwt.pro
	make
	sudo make install
	

## setup project

edit electrocardiogram.pro to set your qwt library path.

![Main Window]https://raw.github.com/crazyleen/Electrocardiagrom/master/mainwindow_view.png)

