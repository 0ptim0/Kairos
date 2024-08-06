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
        series = new QPieSeries();
        chart = new QChart();
        this->setRenderHint(QPainter::Antialiasing);
    }

    bool update(const std::unordered_map<std::string, float> &slices) {
        series->clear();
        chart->removeAllSeries();

        for (auto &slice : slices) {
            series->append(QString::fromStdString(slice.first), slice.second);
        }

        QList<QColor> colors = {QColor(0x1f77b4), QColor(0xff7f0e),
                                QColor(0x2ca02c)};
        int i = 0;
        foreach (QPieSlice *slice, series->slices()) {
            slice->setBrush(colors[i++ % colors.size()]);
            slice->setLabelVisible();
            slice->setLabelColor(Qt::white);
        }

        chart->setMinimumSize(500, 200);
        chart->resize(500, 200);
        chart->setMargins(QMargins(10, 10, 10, 10));

        chart->setBackgroundBrush(Qt::NoBrush);
        chart->setBackgroundPen(Qt::NoPen);
        chart->setBackgroundRoundness(0);

        chart->addSeries(series);
        chart->legend()->hide();
        this->setChart(chart);
        // this->setStyleSheet("background: transparent; border: 0px;");
        return true;
    }

private:
    QChart *chart = nullptr;
    QPieSeries *series = nullptr;
};

#endif  // CHARTVIEW_H
