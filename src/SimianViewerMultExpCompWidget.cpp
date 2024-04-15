#include "SimianViewerMultExpCompViewerWidget.h"
#include "PointData/PointData.h"
#include "ClusterData/ClusterData.h"
#include "util/FileUtil.h"
#include <fstream>
#include <string>
#include <iostream>
#include <QVBoxLayout>
#include <chrono>
#include <cassert>

SimianViewerMultExpCompViewerCommunicationObject::SimianViewerMultExpCompViewerCommunicationObject(SimianViewerMultExpCompViewerWidget* parent)
	:
	_parent(parent)
{

}



void SimianViewerMultExpCompViewerCommunicationObject::js_passSelectionSpecies1ToQt(QString data)
{
	//qDebug() << "\nReceived from Javascript to QT\n";
	_parent->js_passSelectionSpecies1ToQt(data.toStdString());
}


void SimianViewerMultExpCompViewerCommunicationObject::js_passSelectionSpecies2ToQt(QString data)
{
	//qDebug() << "\nReceived from Javascript to QT\n";
	_parent->js_passSelectionSpecies2ToQt(data.toStdString());
}

void SimianViewerMultExpCompViewerCommunicationObject::js_crossspeciesclusterSelection(QString data)
{
	//qDebug() << "\nReceived from Javascript to QT\n";
	_parent->js_crossspeciesclusterSelection(data.toStdString());
}

SimianViewerMultExpCompViewerWidget::SimianViewerMultExpCompViewerWidget() :
	loaded(false)
{
	Q_INIT_RESOURCE(SimianViewerMultExpComp_viewer_resources);
	_communicationObject = new SimianViewerMultExpCompViewerCommunicationObject(this);
	init(_communicationObject);
	setMinimumHeight(140);
}

SimianViewerMultExpCompViewerWidget::~SimianViewerMultExpCompViewerWidget()
{

}

void SimianViewerMultExpCompViewerWidget::setData(std::string visDataContent)
{

	emit _communicationObject->qt_setData(QString((visDataContent).c_str()));
}

void SimianViewerMultExpCompViewerWidget::updateSelectionColor(QString visColorContent)
{
	emit _communicationObject->qt_updateSelectionColor(visColorContent);
}

void SimianViewerMultExpCompViewerWidget::setSpecies1(QString species1)
{

	emit _communicationObject->qt_setSpecies1(species1);
}

void SimianViewerMultExpCompViewerWidget::setSpecies2(QString species2)
{

	emit _communicationObject->qt_setSpecies2(species2);
}

void SimianViewerMultExpCompViewerWidget::setSelectedCrossspeciescluster(QString visDataContent)
{

	emit _communicationObject->qt_setSelectedCrossspeciescluster(visDataContent);
}
//void SimianViewerMultExpCompViewerWidget::setBarhighlight(QString Cluster)
//{
//
//	emit _communicationObject->qt_setBarhighlight(Cluster);
//}

void SimianViewerMultExpCompViewerWidget::js_passSelectionSpecies1ToQt(std::string selectedIDs)
{
	emit passSelectionSpecies1ToQt(selectedIDs);
}

void SimianViewerMultExpCompViewerWidget::js_passSelectionSpecies2ToQt(std::string selectedIDs)
{
	emit passSelectionSpecies2ToQt(selectedIDs);
}

void SimianViewerMultExpCompViewerWidget::js_crossspeciesclusterSelection(std::string selectedIDs)
{
	emit crossspeciesclusterSelection(selectedIDs);
}

void SimianViewerMultExpCompViewerWidget::initWebPage()
{
	loaded = true;

	for (QString option : dataOptionBuffer) {
		emit _communicationObject->qt_addAvailableData(option);
	}
	dataOptionBuffer.clear();

	emit widgetInitialized();
}

void SimianViewerMultExpCompViewerWidget::addDataOption(const QString option)
{
	if (loaded)
		emit _communicationObject->qt_addAvailableData(option);
	else
		dataOptionBuffer.append(option);
}


