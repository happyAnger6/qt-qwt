#include "histplot.h"

#include <QwtPlotLayout>
#include <QwtPlotCurve>
#include <QwtScaleDraw>
#include <QwtScaleMap>
#include <QwtScaleWidget>
#include <QwtLegend>
#include <QwtLegendLabel>
#include <QwtPlotCanvas>
#include <QPainter>

namespace
{
    class TimeScaleDraw : public QwtScaleDraw
    {
      public:
        TimeScaleDraw( const QTime& base )
            : baseTime( base )
        {
        }
        virtual QwtText label( double v ) const QWT_OVERRIDE
        {
            QTime upTime = baseTime.addSecs( static_cast< int >( v ) );
            return upTime.toString();
        }
      private:
        QTime baseTime;
    };

    class Background : public QwtPlotItem
    {
      public:
        Background()
        {
            setZ( 0.0 );
        }

        virtual int rtti() const QWT_OVERRIDE
        {
            return QwtPlotItem::Rtti_PlotUserItem;
        }

        virtual void draw( QPainter* painter,
            const QwtScaleMap&, const QwtScaleMap& yMap,
            const QRectF& canvasRect ) const QWT_OVERRIDE
        {
            QColor c( Qt::white );
            QRectF r = canvasRect;

            for ( int i = 100; i > 0; i -= 10 )
            {
                r.setBottom( yMap.transform( i - 10 ) );
                r.setTop( yMap.transform( i ) );
                painter->fillRect( r, c );

                c = c.darker( 110 );
            }
        }
    };

    class HistCurve : public QwtPlotCurve
    {
      public:
        HistCurve( const QString& title )
            : QwtPlotCurve( title )
        {
          setRenderHint(QwtPlotItem::RenderAntialiased);
        }

        void setColor( const QColor& color )
        {
            QColor c = color;
            c.setAlpha( 150 );

            setPen( QPen( Qt::NoPen ) );
            setBrush( c );
        }
    };
}

HistPlot::HistPlot( QWidget* parent, QTime start_time, int hist_max)
    : QwtPlot(parent)
    , start_time_(start_time), hist_max_(hist_max)
{
	x_datas_ = new double[hist_max];
	y_datas_ = new double[hist_max];
 
    using namespace QwtAxis;
    QwtPlotCanvas* canvas = new QwtPlotCanvas();
    canvas->setBorderRadius( 10 );

    setCanvas(canvas);

    plotLayout()->setAlignCanvasToScales(true);

	setAxisTitle( XBottom, " System Uptime [h:m:s]" );
    setAxisScaleDraw( XBottom, new TimeScaleDraw(start_time_));
    setAxisScale( XBottom, 0, hist_max_ );
    setAxisLabelRotation( XBottom, -40.0 );
    setAxisLabelAlignment( XBottom, Qt::AlignLeft | Qt::AlignBottom );

 /*   QwtScaleWidget* scaleWidget = axisWidget( XBottom );
    const int fmh = QFontMetrics( scaleWidget->font() ).height();
    scaleWidget->setMinBorderDist( 0, fmh / 2 ); */

    setAxisTitle( YLeft, "Mem Usage [%]" );
    setAxisScale( YLeft, 0, 100);

    Background* bg = new Background();
    bg->attach( this );

	HistCurve *curve;
    curve = new HistCurve( "Mem" );
    curve->setColor( Qt::red );
    curve->attach( this );
	curve_ = curve;

	for (int i = 0; i < hist_max_; i++)
		y_datas_[hist_max_ - 1 - i] = i; 
}

int HistPlot::add_sample(double x, double y)
{
	for (int i = data_cnt_; i > 0; i--) {
		if (i < hist_max_) {
			x_datas_[i] = x_datas_[i-1];
		}
	}

	x_datas_[0] = x;

	for (int j = 0; j < hist_max_; j++)
		y_datas_[j]++;

	if (data_cnt_ < hist_max_)
		data_cnt_++;
	
	setAxisScale( QwtAxis::XBottom, y_datas_[hist_max_-1], y_datas_[0]);
    curve_->setRawSamples(y_datas_, x_datas_, data_cnt_);

    replot();
	return 0;
}