#ifndef TRACKITEM_H
#define TRACKITEM_H

#include <QDateTime>
#include <QPen>
#include "data/track.h"
#include "pathitem.h"
#include "units.h"

class Map;

class TrackItem : public PathItem
{
	Q_OBJECT

public:
    TrackItem(const Track &track, Map *map, QGraphicsItem *parent = 0);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
      QWidget *widget);

	void setUnits(Units units);

private:
	QString toolTip(Units units) const;

    const Track _track;
    qreal _vel_max;

	QString _name;
	QString _desc;
	QDateTime _date;
	qreal _time;
	qreal _movingTime;

    const static QVector<QRgb> _ctable;
};

#endif // TRACKITEM_H
