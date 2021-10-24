#ifndef HISTPLOT_H
#define HISTPLOT_H

#include <QwtPlot>
#include <QwtPlotCurve>
#include <QTime>

#define DEFAULT_HIST 60

class HistPlot : public QwtPlot {
    Q_OBJECT
  public:
    HistPlot( QWidget* = 0 , QTime start_time_ = QTime(0, 0, 0), int hist_max=DEFAULT_HIST);
	int add_sample(double x, double y);

  private:
    QwtPlotCurve* curve_;
	double *x_datas_;
	double *y_datas_;
	QTime start_time_;
	int hist_max_;
	int data_cnt_ = 0;
};

#endif
