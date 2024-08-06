#ifndef CHARTVIEW_H
#define CHARTVIEW_H

#include <QWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <unordered_map>

class ChartWidget : public QChartView {
    Q_OBJECT

public:
    explicit ChartWidget(QWidget *parent = nullptr) : QChartView(parent) {
        chart = new QChart();
        this->setRenderHint(QPainter::Antialiasing);
    }

    bool update(const std::unordered_map<QString, float> &slices) {
        if (series != nullptr) {
            // chart takes ownership of the series object
            chart->removeAllSeries();
        }
        series = new QPieSeries();
    
        for (auto &slice : slices) {
            series->append(slice.first, slice.second);
        }

        int i = 0;
        foreach (QPieSlice *slice, series->slices()) {
            slice->setLabelVisible();
            slice->setLabelColor(Qt::white);
            QFont labelFont = slice->labelFont();
            labelFont.setPointSize(8);
            slice->setLabelFont(labelFont);
        }

        // chart->setMinimumSize(500, 200);
        // chart->resize(500, 200);
        // this->setFixedSize(500, 200);
        // this->setAlignment(Qt::AlignLeft);
        chart->setMargins(QMargins(1, 1, 1, 1));

        chart->setBackgroundBrush(Qt::NoBrush);
        chart->setBackgroundPen(Qt::NoPen);
        chart->setBackgroundRoundness(0);

        chart->addSeries(series);

        chart->legend()->hide();
        // QLegend *legend = chart->legend();
        // legend->setVisible(true);
        // legend->setLabelColor(Qt::white);
        // legend->setMarkerShape(QLegend::MarkerShapeFromSeries);
        this->setChart(chart);
        return true;
    }

private:
    QChart *chart = nullptr;
    QPieSeries *series = nullptr;
};

#endif  // CHARTVIEW_H
