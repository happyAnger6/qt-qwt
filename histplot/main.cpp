/*****************************************************************************
 * Qwt Examples - Copyright (C) 2002 Uwe Rathmann
 * This file may be used under the terms of the 3-clause BSD License
 *****************************************************************************/

#include "memplot.h"

#include <QApplication>
#include <QLayout>
#include <QLabel>

int main( int argc, char** argv )
{
    QApplication app( argc, argv );

    QWidget vBox;
    vBox.setWindowTitle( "Mem Plot" );

    MemPlot* plot = new MemPlot( &vBox );
    plot->setTitle( "History" );

    const int margin = 5;
    plot->setContentsMargins( margin, margin, margin, margin );

    QVBoxLayout* layout = new QVBoxLayout( &vBox );
    layout->addWidget( plot );

    vBox.resize( 600, 400 );
    vBox.show();

    return app.exec();
}
