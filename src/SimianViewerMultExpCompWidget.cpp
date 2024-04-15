#include "SimianViewerMultExpCompWidget.h"
#include "PointData/PointData.h"
#include "ClusterData/ClusterData.h"
#include "util/FileUtil.h"
#include <fstream>
#include <string>
#include <iostream>
#include <QVBoxLayout>
#include <chrono>
#include <cassert>

SimianViewerMultExpCompCommunicationObject::SimianViewerMultExpCompCommunicationObject(SimianViewerMultExpCompWidget* parent)
	:
	_parent(parent)
{

}



void SimianViewerMultExpCompCommunicationObject::js_passSelectionSpecies1ToQt(QString data)
{
	//qDebug() << "\nReceived from Javascript to QT\n";
	_parent->js_passSelectionSpecies1ToQt(data.toStdString());
}


void SimianViewerMultExpCompCommunicationObject::js_passSelectionSpecies2ToQt(QString data)
{
	//qDebug() << "\nReceived from Javascript to QT\n";
	_parent->js_passSelectionSpecies2ToQt(data.toStdString());
}

void SimianViewerMultExpCompCommunicationObject::js_crossspeciesclusterSelection(QString data)
{
	//qDebug() << "\nReceived from Javascript to QT\n";
	_parent->js_crossspeciesclusterSelection(data.toStdString());
}

SimianViewerMultExpCompWidget::SimianViewerMultExpCompWidget() :
	loaded(false)
{
	Q_INIT_RESOURCE(SimianViewerMultExpComp_viewer_resources);
	_communicationObject = new SimianViewerMultExpCompCommunicationObject(this);
	init(_communicationObject);
	setMinimumHeight(140);
}

SimianViewerMultExpCompWidget::~SimianViewerMultExpCompWidget()
{

}

void SimianViewerMultExpCompWidget::setData(std::string visDataContent)
{

	emit _communicationObject->qt_setData(QString((visDataContent).c_str()));
}

void SimianViewerMultExpCompWidget::updateSelectionColor(QString visColorContent)
{
	emit _communicationObject->qt_updateSelectionColor(visColorContent);
}

void SimianViewerMultExpCompWidget::setSpecies1(QString species1)
{

	emit _communicationObject->qt_setSpecies1(species1);
}

void SimianViewerMultExpCompWidget::setSpecies2(QString species2)
{

	emit _communicationObject->qt_setSpecies2(species2);
}

void SimianViewerMultExpCompWidget::setSelectedCrossspeciescluster(QString visDataContent)
{

	emit _communicationObject->qt_setSelectedCrossspeciescluster(visDataContent);
}
//void SimianViewerMultExpCompWidget::setBarhighlight(QString Cluster)
//{
//
//	emit _communicationObject->qt_setBarhighlight(Cluster);
//}

void SimianViewerMultExpCompWidget::js_passSelectionSpecies1ToQt(std::string selectedIDs)
{
	emit passSelectionSpecies1ToQt(selectedIDs);
}

void SimianViewerMultExpCompWidget::js_passSelectionSpecies2ToQt(std::string selectedIDs)
{
	emit passSelectionSpecies2ToQt(selectedIDs);
}

void SimianViewerMultExpCompWidget::js_crossspeciesclusterSelection(std::string selectedIDs)
{
	emit crossspeciesclusterSelection(selectedIDs);
}

void SimianViewerMultExpCompWidget::initWebPage()
{
	loaded = true;

	for (QString option : dataOptionBuffer) {
		emit _communicationObject->qt_addAvailableData(option);
	}
	dataOptionBuffer.clear();

	emit widgetInitialized();
}

void SimianViewerMultExpCompWidget::addDataOption(const QString option)
{
	if (loaded)
		emit _communicationObject->qt_addAvailableData(option);
	else
		dataOptionBuffer.append(option);
}


