/*****************************************************************************
 * Qwt Examples - Copyright (C) 2002 Uwe Rathmann
 * This file may be used under the terms of the 3-clause BSD License
 *****************************************************************************/

#pragma once

#include <QTime>

#include "histplot.h"

class MemPlot : public HistPlot
{
	public:
		MemPlot(QWidget *parent = nullptr);

	protected:
          void timerEvent(QTimerEvent *) QWT_OVERRIDE;

    private:
          enum Value { Total, Free, Avail, NValues };
          void lookUp(double[NValues]) const;
};
