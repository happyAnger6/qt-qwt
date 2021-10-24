/*****************************************************************************
 * Qwt Examples - Copyright (C) 2002 Uwe Rathmann
 * This file may be used under the terms of the 3-clause BSD License
 *****************************************************************************/

#include "memplot.h"

#include <QStringList>
#include <QFile>
#include <QTextStream>

MemPlot::MemPlot(QWidget *parent)
	: HistPlot(parent)
{
    (void)startTimer(1000);
}

void MemPlot::timerEvent( QTimerEvent* )
{
	static double time_ = 0;
    double values[NValues];
    lookUp( values );
    double usage = (values[Total] - values[Avail])/ values[Total] * 100.0;

	time_++;
	add_sample(usage, time_);
}

void MemPlot::lookUp( double values[NValues] ) const
{
    QFile file( "/proc/meminfo" );
    file.open( QIODevice::ReadOnly );
    QTextStream textStream( &file );
    do
    {
        QString line = textStream.readLine();
        line = line.trimmed();
        const QStringList valueList =line.split( " ",  QString::SkipEmptyParts);
        if ( line.startsWith( "MemTotal" ) )
        {
            values[Total] = valueList[1].toDouble();
        } else if (line.startsWith( "MemFree" ) ) {
            values[Free] = valueList[1].toDouble();
        } else if (line.startsWith( "MemAvailable" ) ) {
            values[Avail] = valueList[1].toDouble();
        } else {
            break;
        }
    } while( !textStream.atEnd() );
}
