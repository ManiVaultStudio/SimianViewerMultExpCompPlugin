#include "ParallelBarsViewerWidget.h"
#include "PointData.h"
#include "ClusterData.h"
#include "util/FileUtil.h"
#include <fstream>
#include <string>
#include <iostream>
#include <QVBoxLayout>
#include <chrono>
#include <cassert>

ParallelBarsViewerCommunicationObject::ParallelBarsViewerCommunicationObject(ParallelBarsViewerWidget* parent)
	:
	_parent(parent)
{

}



void ParallelBarsViewerCommunicationObject::js_passSelectionSpecies1ToQt(QString data)
{
	//qDebug() << "\nReceived from Javascript to QT\n";
	_parent->js_passSelectionSpecies1ToQt(data.toStdString());
}


void ParallelBarsViewerCommunicationObject::js_passSelectionSpecies2ToQt(QString data)
{
	//qDebug() << "\nReceived from Javascript to QT\n";
	_parent->js_passSelectionSpecies2ToQt(data.toStdString());
}

void ParallelBarsViewerCommunicationObject::js_crossspeciesclusterSelection(QString data)
{
	//qDebug() << "\nReceived from Javascript to QT\n";
	_parent->js_crossspeciesclusterSelection(data.toStdString());
}

ParallelBarsViewerWidget::ParallelBarsViewerWidget() :
	loaded(false)
{
	Q_INIT_RESOURCE(ParallelBars_viewer_resources);
	_communicationObject = new ParallelBarsViewerCommunicationObject(this);
	init(_communicationObject);
	setMinimumHeight(140);
}

ParallelBarsViewerWidget::~ParallelBarsViewerWidget()
{

}

void ParallelBarsViewerWidget::setData(std::string visDataContent)
{

	emit _communicationObject->qt_setData(QString((visDataContent).c_str()));
}

void ParallelBarsViewerWidget::updateSelectionColor(QString visColorContent)
{
	emit _communicationObject->qt_updateSelectionColor(visColorContent);
}

void ParallelBarsViewerWidget::setSpecies1(QString species1)
{

	emit _communicationObject->qt_setSpecies1(species1);
}

void ParallelBarsViewerWidget::setSpecies2(QString species2)
{

	emit _communicationObject->qt_setSpecies2(species2);
}

void ParallelBarsViewerWidget::setSelectedCrossspeciescluster(QString visDataContent)
{

	emit _communicationObject->qt_setSelectedCrossspeciescluster(visDataContent);
}
//void ParallelBarsViewerWidget::setBarhighlight(QString Cluster)
//{
//
//	emit _communicationObject->qt_setBarhighlight(Cluster);
//}

void ParallelBarsViewerWidget::js_passSelectionSpecies1ToQt(std::string selectedIDs)
{
	emit passSelectionSpecies1ToQt(selectedIDs);
}

void ParallelBarsViewerWidget::js_passSelectionSpecies2ToQt(std::string selectedIDs)
{
	emit passSelectionSpecies2ToQt(selectedIDs);
}

void ParallelBarsViewerWidget::js_crossspeciesclusterSelection(std::string selectedIDs)
{
	emit crossspeciesclusterSelection(selectedIDs);
}

void ParallelBarsViewerWidget::initWebPage()
{
	loaded = true;

	for (QString option : dataOptionBuffer) {
		emit _communicationObject->qt_addAvailableData(option);
	}
	dataOptionBuffer.clear();

	emit widgetInitialized();
}

void ParallelBarsViewerWidget::addDataOption(const QString option)
{
	if (loaded)
		emit _communicationObject->qt_addAvailableData(option);
	else
		dataOptionBuffer.append(option);
}



