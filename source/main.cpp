// STD
#include <iostream>
// Qt
#include <QApplication>
//troen
#include "MainWindow.h"
#include "troengame.h"

#define EXTERN
#include "globals.h"


#include <stdio.h>
#include <string.h>


//#include "network\networkmanager.h"



using namespace troen;

/*! \file The main file is the entry point of the c++ application and sets up the Qt app (see mainwindow.cpp).*/

int main(int argc, char* argv[])
{


	int result = -1;

	// register meta types
	qRegisterMetaType<troen::GameConfig>("GameConfig");

	// setup application settings
	QApplication::setApplicationName("Troen");
	QApplication * application = new QApplication(argc, argv);

	troen::MainWindow * mainWindow = new troen::MainWindow();
	mainWindow->show();

	result = application->exec();

	// Clean Up
	delete mainWindow;
	delete application;

	return result;
}