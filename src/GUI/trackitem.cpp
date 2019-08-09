#include <QPainter>
#include "map/map.h"
#include "format.h"
#include "tooltip.h"
#include "trackitem.h"


QString TrackItem::toolTip(Units units) const
{
	ToolTip tt;

	if (!_name.isEmpty())
		tt.insert(tr("Name"), _name);
	if (!_desc.isEmpty())
		tt.insert(tr("Description"), _desc);
	tt.insert(tr("Distance"), Format::distance(path().last().last().distance(),
	  units));
	if  (_time > 0)
		tt.insert(tr("Total time"), Format::timeSpan(_time));
	if  (_movingTime > 0)
		tt.insert(tr("Moving time"), Format::timeSpan(_movingTime));
	if (!_date.isNull())
		tt.insert(tr("Date"), _date.toString(Qt::SystemLocaleShortDate));

	return tt.toString();
}

TrackItem::TrackItem(const Track &track, Map *map, QGraphicsItem *parent)
  : PathItem(track.path(), map, parent), _track(track)
{
	_name = track.name();
	_desc = track.description();
	_date = track.date();
	_time = track.time();
	_movingTime = track.movingTime();

    _vel_max = 0.0;
    for (int i = 0; i<_track._data.size(); ++i) {
        const SegmentData &sd = _track._data.at(i);
        const Track::Segment &seg = _track._segments.at(i);
        for (int j = 0; j < sd.size(); ++j) {
            if (!seg.outliers.contains(j) && !_track.discardStopPoint(seg, j)) {
                _vel_max = seg.speed.at(j)>_vel_max ? seg.speed.at(j) : _vel_max;
            }
        }
    }
    _vel_max *= 1.01;

	setToolTip(toolTip(Metric));
}

void TrackItem::setUnits(Units units)
{
	PathItem::setUnits(units);
	setToolTip(toolTip(units));
}

void TrackItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
      QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QPointF point, old_point;
    QColor old_color = _pen.color();

    painter->setPen(_pen);
    for (int i = 0; i < _track._data.size(); ++i) {
        const SegmentData &sd = _track._data.at(i);
        if (sd.size() < 2) {
            continue;
        }
        const Track::Segment &seg = _track._segments.at(i);
        bool is_first = true;
        for (int j = 0; j < sd.size(); ++j) {
            if (!seg.outliers.contains(j) && !_track.discardStopPoint(seg, j)) {
                point = _map->ll2xy(sd.at(j).coordinates());
                if (!is_first) {
                    _pen.setColor(_ctable.at(int(_ctable.size()*seg.speed.at(j)/_vel_max)));
                    painter->setPen(_pen);
                    painter->drawLine(old_point,point);
                } else {
                    is_first = false;
                }
                old_point = point;
            }
        }
    }
    _pen.setColor(old_color);

/*
    painter->setPen(Qt::red);
    painter->drawRect(boundingRect());
*/
}

const QVector<QRgb> TrackItem::_ctable = QVector<QRgb>()
        << qRgb(0,0,0) <<
           qRgb(3,1,1) <<
           qRgb(6,2,1) <<
           qRgb(10,3,2) <<
           qRgb(13,4,2) <<
           qRgb(15,5,3) <<
           qRgb(17,6,3) <<
           qRgb(20,7,4) <<
           qRgb(21,8,4) <<
           qRgb(23,9,5) <<
           qRgb(25,10,5) <<
           qRgb(26,11,6) <<
           qRgb(28,11,6) <<
           qRgb(29,12,7) <<
           qRgb(31,13,8) <<
           qRgb(32,14,8) <<
           qRgb(33,15,9) <<
           qRgb(35,15,9) <<
           qRgb(36,16,10) <<
           qRgb(37,16,10) <<
           qRgb(39,17,11) <<
           qRgb(40,17,11) <<
           qRgb(42,18,12) <<
           qRgb(43,18,12) <<
           qRgb(45,18,13) <<
           qRgb(46,19,13) <<
           qRgb(48,19,14) <<
           qRgb(49,19,14) <<
           qRgb(51,20,15) <<
           qRgb(53,20,15) <<
           qRgb(54,20,15) <<
           qRgb(56,21,16) <<
           qRgb(57,21,16) <<
           qRgb(59,21,17) <<
           qRgb(60,22,17) <<
           qRgb(62,22,17) <<
           qRgb(64,22,18) <<
           qRgb(65,23,18) <<
           qRgb(67,23,18) <<
           qRgb(68,23,19) <<
           qRgb(70,24,19) <<
           qRgb(72,24,19) <<
           qRgb(73,24,19) <<
           qRgb(75,24,20) <<
           qRgb(76,25,20) <<
           qRgb(78,25,20) <<
           qRgb(80,25,21) <<
           qRgb(81,26,21) <<
           qRgb(83,26,21) <<
           qRgb(85,26,21) <<
           qRgb(86,26,22) <<
           qRgb(88,27,22) <<
           qRgb(90,27,22) <<
           qRgb(91,27,22) <<
           qRgb(93,27,23) <<
           qRgb(95,28,23) <<
           qRgb(96,28,23) <<
           qRgb(98,28,24) <<
           qRgb(100,28,24) <<
           qRgb(102,28,24) <<
           qRgb(103,29,24) <<
           qRgb(105,29,25) <<
           qRgb(107,29,25) <<
           qRgb(109,29,25) <<
           qRgb(110,29,25) <<
           qRgb(112,30,26) <<
           qRgb(114,30,26) <<
           qRgb(115,30,26) <<
           qRgb(117,30,26) <<
           qRgb(119,30,27) <<
           qRgb(121,30,27) <<
           qRgb(122,31,27) <<
           qRgb(124,31,28) <<
           qRgb(126,31,28) <<
           qRgb(128,31,28) <<
           qRgb(130,31,28) <<
           qRgb(131,31,29) <<
           qRgb(133,32,29) <<
           qRgb(135,32,29) <<
           qRgb(137,32,29) <<
           qRgb(139,32,30) <<
           qRgb(140,32,30) <<
           qRgb(142,32,30) <<
           qRgb(144,32,30) <<
           qRgb(146,32,31) <<
           qRgb(148,32,31) <<
           qRgb(149,33,31) <<
           qRgb(151,33,32) <<
           qRgb(153,33,32) <<
           qRgb(155,33,32) <<
           qRgb(157,33,32) <<
           qRgb(159,33,33) <<
           qRgb(160,33,33) <<
           qRgb(162,33,33) <<
           qRgb(164,33,33) <<
           qRgb(166,33,34) <<
           qRgb(168,33,34) <<
           qRgb(170,33,34) <<
           qRgb(172,33,35) <<
           qRgb(173,33,35) <<
           qRgb(175,34,35) <<
           qRgb(176,36,35) <<
           qRgb(177,38,35) <<
           qRgb(178,39,34) <<
           qRgb(179,41,34) <<
           qRgb(180,43,34) <<
           qRgb(181,45,34) <<
           qRgb(182,46,33) <<
           qRgb(183,48,33) <<
           qRgb(184,50,33) <<
           qRgb(185,51,33) <<
           qRgb(186,53,32) <<
           qRgb(187,55,32) <<
           qRgb(188,56,32) <<
           qRgb(189,58,31) <<
           qRgb(189,59,31) <<
           qRgb(190,61,31) <<
           qRgb(191,62,30) <<
           qRgb(192,64,30) <<
           qRgb(193,65,29) <<
           qRgb(194,66,29) <<
           qRgb(195,68,29) <<
           qRgb(196,69,28) <<
           qRgb(197,71,28) <<
           qRgb(198,72,27) <<
           qRgb(199,73,27) <<
           qRgb(200,75,26) <<
           qRgb(201,76,26) <<
           qRgb(202,78,25) <<
           qRgb(203,79,25) <<
           qRgb(204,80,24) <<
           qRgb(205,82,24) <<
           qRgb(206,83,23) <<
           qRgb(207,84,22) <<
           qRgb(208,86,22) <<
           qRgb(208,87,21) <<
           qRgb(209,88,20) <<
           qRgb(210,90,19) <<
           qRgb(211,91,19) <<
           qRgb(212,92,18) <<
           qRgb(213,94,17) <<
           qRgb(214,95,16) <<
           qRgb(215,96,15) <<
           qRgb(216,98,14) <<
           qRgb(217,99,12) <<
           qRgb(218,100,11) <<
           qRgb(219,102,10) <<
           qRgb(220,103,8) <<
           qRgb(221,104,7) <<
           qRgb(222,105,5) <<
           qRgb(222,107,5) <<
           qRgb(222,109,5) <<
           qRgb(223,111,5) <<
           qRgb(223,113,5) <<
           qRgb(223,114,5) <<
           qRgb(223,116,5) <<
           qRgb(223,118,4) <<
           qRgb(224,120,4) <<
           qRgb(224,121,4) <<
           qRgb(224,123,4) <<
           qRgb(224,125,4) <<
           qRgb(225,126,4) <<
           qRgb(225,128,4) <<
           qRgb(225,130,4) <<
           qRgb(225,131,4) <<
           qRgb(225,133,4) <<
           qRgb(226,135,4) <<
           qRgb(226,136,4) <<
           qRgb(226,138,4) <<
           qRgb(226,140,4) <<
           qRgb(226,141,4) <<
           qRgb(226,143,4) <<
           qRgb(227,145,3) <<
           qRgb(227,146,3) <<
           qRgb(227,148,3) <<
           qRgb(227,150,3) <<
           qRgb(227,151,3) <<
           qRgb(227,153,3) <<
           qRgb(227,154,3) <<
           qRgb(228,156,3) <<
           qRgb(228,158,3) <<
           qRgb(228,159,3) <<
           qRgb(228,161,3) <<
           qRgb(228,162,3) <<
           qRgb(228,164,3) <<
           qRgb(228,166,3) <<
           qRgb(228,167,3) <<
           qRgb(228,169,3) <<
           qRgb(228,170,3) <<
           qRgb(229,172,3) <<
           qRgb(229,173,4) <<
           qRgb(229,175,4) <<
           qRgb(229,177,4) <<
           qRgb(229,178,4) <<
           qRgb(229,180,4) <<
           qRgb(229,181,4) <<
           qRgb(229,183,4) <<
           qRgb(229,184,4) <<
           qRgb(229,186,4) <<
           qRgb(229,187,4) <<
           qRgb(229,189,4) <<
           qRgb(229,191,4) <<
           qRgb(229,192,4) <<
           qRgb(229,194,4) <<
           qRgb(229,195,5) <<
           qRgb(229,197,5) <<
           qRgb(229,198,5) <<
           qRgb(229,200,5) <<
           qRgb(229,201,5) <<
           qRgb(229,203,5) <<
           qRgb(229,205,5) <<
           qRgb(229,206,6) <<
           qRgb(229,208,6) <<
           qRgb(229,209,6) <<
           qRgb(229,211,6) <<
           qRgb(229,212,17) <<
           qRgb(230,213,32) <<
           qRgb(231,214,43) <<
           qRgb(232,215,52) <<
           qRgb(233,216,60) <<
           qRgb(234,217,67) <<
           qRgb(235,218,74) <<
           qRgb(236,219,81) <<
           qRgb(237,220,87) <<
           qRgb(238,221,93) <<
           qRgb(239,222,99) <<
           qRgb(240,223,105) <<
           qRgb(241,224,110) <<
           qRgb(242,225,116) <<
           qRgb(243,227,121) <<
           qRgb(244,228,127) <<
           qRgb(244,229,132) <<
           qRgb(245,230,137) <<
           qRgb(246,231,143) <<
           qRgb(247,232,148) <<
           qRgb(247,233,153) <<
           qRgb(248,234,158) <<
           qRgb(249,235,163) <<
           qRgb(249,236,168) <<
           qRgb(250,237,173) <<
           qRgb(250,238,179) <<
           qRgb(251,239,184) <<
           qRgb(251,241,189) <<
           qRgb(252,242,194) <<
           qRgb(252,243,199) <<
           qRgb(253,244,204) <<
           qRgb(253,245,209) <<
           qRgb(253,246,214) <<
           qRgb(254,247,219) <<
           qRgb(254,248,224) <<
           qRgb(254,249,229) <<
           qRgb(254,251,235) <<
           qRgb(255,252,240) <<
           qRgb(255,253,245) <<
           qRgb(255,254,250) <<
           qRgb(255,255,255);
