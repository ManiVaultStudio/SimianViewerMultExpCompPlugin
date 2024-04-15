#include "SimianViewerMultExpCompViewerPlugin.h"

#include "PointData/PointData.h"
#include "event/Event.h"
#include "ColorData/ColorData.h"

#include <actions/PluginTriggerAction.h>

#include <QtCore>
#include <QtDebug>
#include <QWebEngineView>
#include<ios>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

Q_PLUGIN_METADATA(IID "nl.tudelft.SimianViewerMultExpCompViewerPlugin")

using namespace mv;

// =============================================================================
// View
// =============================================================================

SimianViewerMultExpCompViewerPlugin::SimianViewerMultExpCompViewerPlugin(const PluginFactory* factory) :
	ViewPlugin(factory),
	_SimianViewerMultExpComp_viewer(),
	_SimianViewerMultExpCompOptionsAction(*this, _core)
{
	setSerializationName("SimianViewerMultExpCompViewer");

}

SimianViewerMultExpCompViewerPlugin::~SimianViewerMultExpCompViewerPlugin()
{
}

void SimianViewerMultExpCompViewerPlugin::init()
{
	connect(&_SimianViewerMultExpComp_viewer, &SimianViewerMultExpCompViewerWidget::widgetInitialized, &_SimianViewerMultExpCompOptionsAction, &SimianViewerMultExpCompOptionsAction::initLoader);
	_SimianViewerMultExpComp_viewer.setPage(":/SimianViewerMultExpComp_viewer/SimianViewerMultExpComp_viewer.html", "qrc:/SimianViewerMultExpComp_viewer/");
	_SimianViewerMultExpComp_viewer.setContentsMargins(0, 0, 0, 0);
	_SimianViewerMultExpComp_viewer.layout()->setContentsMargins(0, 0, 0, 0);
	//_SimianViewerMultExpCompOptionsAction = new SimianViewerMultExpCompOptionsAction(*this, _core);
	connect(&_SimianViewerMultExpComp_viewer, &SimianViewerMultExpCompViewerWidget::passSelectionSpecies1ToQt, this, &SimianViewerMultExpCompViewerPlugin::publishSelectionSpecies1);

	connect(&_SimianViewerMultExpComp_viewer, &SimianViewerMultExpCompViewerWidget::passSelectionSpecies2ToQt, this, &SimianViewerMultExpCompViewerPlugin::publishSelectionSpecies2);

	connect(&_SimianViewerMultExpComp_viewer, &SimianViewerMultExpCompViewerWidget::crossspeciesclusterSelection, this, &SimianViewerMultExpCompViewerPlugin::clusterSelection);

	_eventListener.addSupportedEventType(static_cast<std::uint32_t>(EventType::DatasetDataSelectionChanged));
	_eventListener.registerDataEventByType(ClusterType, std::bind(&SimianViewerMultExpCompViewerPlugin::onDataEvent, this, std::placeholders::_1));

	auto topToolbarWidget = new QWidget();
	auto topToolbarLayout = new QHBoxLayout();
	topToolbarLayout->setContentsMargins(0, 0, 0, 0);
	topToolbarLayout->setSpacing(0);

	auto ClusterDataset1SelectionWidget = _SimianViewerMultExpCompOptionsAction.getdeStatsDataset1SelectionAction().createCollapsedWidget(&getWidget());
	//ClusterDataset1SelectionWidget->setMaximumWidth(280);
	//topToolbarLayout->addWidget(ClusterDataset1SelectionWidget);
	//auto humancompTempWidget = _SimianViewerMultExpCompOptionsAction.getHumancomparisonAction().createWidget(&getWidget());
	//humancompTempWidget->setFixedWidth(270);
	//topToolbarLayout->addWidget(humancompTempWidget);
	topToolbarLayout->addWidget(&_SimianViewerMultExpCompOptionsAction.getRadioButtonforAllSpeciesGeneExpression());
	topToolbarLayout->addWidget(&_SimianViewerMultExpCompOptionsAction.getRadioButtonforHumandifferentialExpression());
	//topToolbarLayout->addSpacing(10);
	//topToolbarLayout->addWidget(_SimianViewerMultExpCompOptionsAction.getHumancomparisonAbsoluteValuesAction().createWidget(&getWidget()));
	//auto ClusterDataset2SelectionWidget = _SimianViewerMultExpCompOptionsAction.getdeStatsDataset2SelectionAction().createCollapsedWidget(&getWidget());
	////ClusterDataset2SelectionWidget->setMaximumWidth(280);
	//topToolbarLayout->addWidget(ClusterDataset2SelectionWidget);

	//topToolbarLayout->addWidget(_SimianViewerMultExpCompOptionsAction.getScreenshotAction().createWidget(&getWidget()));
	//topToolbarLayout->addWidget(_SimianViewerMultExpCompOptionsAction.getHelpAction().createWidget(&getWidget()));

	topToolbarLayout->addStretch(0);
	topToolbarWidget->setAutoFillBackground(true);
	topToolbarWidget->setLayout(topToolbarLayout);
	auto layout = new QVBoxLayout();
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);
	layout->addWidget(topToolbarWidget);
	layout->addWidget(&_SimianViewerMultExpComp_viewer, 1);
	getWidget().setLayout(layout);


}

void SimianViewerMultExpCompViewerPlugin::onDataEvent(mv::DatasetEvent* dataEvent)
{
	if (dataEvent->getType() == mv::EventType::DatasetDataSelectionChanged)
	{


	}
}

void SimianViewerMultExpCompViewerPlugin::publishSelectionSpecies1(std::string clusterName)
{

	//qDebug() << QString::fromStdString(selectedIDs);
	//_SimianViewerMultExpCompOptionsAction.getCrossSpecies1HeatMapCellAction().setCurrentText("");
	//_SimianViewerMultExpCompOptionsAction.getCrossSpecies1HeatMapCellAction().setCurrentText(QString::fromStdString(clusterName));
	auto dataset = _SimianViewerMultExpCompOptionsAction.getdeStatsDataset1SelectAction().getCurrentDataset();
	const auto candidateDataset = mv::data().getDataset<Clusters>(dataset.getDatasetId());
	std::vector<std::uint32_t> selectedIndices;
	for (const auto& cluster : candidateDataset->getClusters())
	{
		if (cluster.getName() == QString::fromStdString(clusterName))
		{
			for (const auto& index : cluster.getIndices())
			{
				selectedIndices.push_back(index);
			}
		}

	}

	candidateDataset->getParent()->setSelectionIndices(selectedIndices);


	events().notifyDatasetDataSelectionChanged(candidateDataset->getParent());

}

void SimianViewerMultExpCompViewerPlugin::clusterSelection(std::string clusterName)
{
	_SimianViewerMultExpCompOptionsAction.getSelectedCrossspeciesclusterFlag() = false;
	if (clusterName == "")
	{
		QString::fromStdString(clusterName);
		_SimianViewerMultExpCompOptionsAction.getSelectedCrossspeciescluster().setString("");
	}
	else
	{
		//qDebug() << QString::fromStdString(clusterName);
		_SimianViewerMultExpCompOptionsAction.getSelectedCrossspeciescluster().setString(QString::fromStdString(clusterName));
	}


}


void SimianViewerMultExpCompViewerPlugin::publishSelectionSpecies2(std::string clusterName)
{

	//qDebug() << QString::fromStdString(selectedIDs);
	//_SimianViewerMultExpCompOptionsAction.getCrossSpecies2HeatMapCellAction().setCurrentText("");
	//_SimianViewerMultExpCompOptionsAction.getCrossSpecies2HeatMapCellAction().setCurrentText(QString::fromStdString(clusterName));
	auto dataset = _SimianViewerMultExpCompOptionsAction.getdeStatsDataset2SelectAction().getCurrentDataset();
	const auto candidateDataset = mv::data().getDataset<Clusters>(dataset.getDatasetId());
	std::vector<std::uint32_t> selectedIndices;
	for (const auto& cluster : candidateDataset->getClusters())
	{
		if (cluster.getName() == QString::fromStdString(clusterName))
		{
			for (const auto& index : cluster.getIndices())
			{
				selectedIndices.push_back(index);
			}
		}

	}

	candidateDataset->getParent()->setSelectionIndices(selectedIndices);


	events().notifyDatasetDataSelectionChanged(candidateDataset->getParent());

}

void SimianViewerMultExpCompViewerPlugin::fromVariantMap(const QVariantMap& variantMap)
{
	ViewPlugin::fromVariantMap(variantMap);

	_SimianViewerMultExpCompOptionsAction.fromParentVariantMap(variantMap);
}

QVariantMap SimianViewerMultExpCompViewerPlugin::toVariantMap() const
{
	QVariantMap variantMap = ViewPlugin::toVariantMap();

	_SimianViewerMultExpCompOptionsAction.insertIntoVariantMap(variantMap);

	return variantMap;
}
//const auto showHelpbox = []() -> void
//{
//	qDebug() << "Population pyramid viewer plugin help requested...";
//	QMessageBox msgBox;
//	msgBox.setTextFormat(Qt::RichText); // this does the magic trick and allows you to click the link
//	msgBox.setText("<div>Population pyramid viewer</div>");
//	//msgBox.setIcon(yourIcon);
//	msgBox.exec();
//};

//SimianViewerMultExpCompViewerPluginFactory::SimianViewerMultExpCompViewerPluginFactory() :
//	ViewPluginFactory()
//{
//	connect(&getTriggerHelpAction(), &TriggerAction::triggered, this, [this]() -> void {
//		// Do your stuff here
//
//		showHelpbox();
//
//		});
//}

// =============================================================================
// Factory
// =============================================================================


QIcon SimianViewerMultExpCompViewerPluginFactory::getIcon(const QColor& color /*= Qt::black*/) const
{
	return Application::getIconFont("FontAwesome").getIcon("chart-bar", color);
}

ViewPlugin* SimianViewerMultExpCompViewerPluginFactory::produce()
{
	return new SimianViewerMultExpCompViewerPlugin(this);
}

mv::DataTypes SimianViewerMultExpCompViewerPluginFactory::supportedDataTypes() const
{
	DataTypes supportedTypes;
	return supportedTypes;
}


mv::gui::PluginTriggerActions SimianViewerMultExpCompViewerPluginFactory::getPluginTriggerActions(const mv::Datasets& datasets) const
{
	PluginTriggerActions pluginTriggerActions;

	const auto getInstance = [this]() -> SimianViewerMultExpCompViewerPlugin* {
		return dynamic_cast<SimianViewerMultExpCompViewerPlugin*>(plugins().requestViewPlugin(getKind()));
	};

	const auto numberOfDatasets = datasets.count();

	if (PluginFactory::areAllDatasetsOfTheSameType(datasets, PointType)) {
		if (numberOfDatasets >= 1) {
			if (datasets.first()->getDataType() == PointType) {
				auto pluginTriggerAction = new PluginTriggerAction(const_cast<SimianViewerMultExpCompViewerPluginFactory*>(this), this, "SimianViewerMultExpComp viewer", "Load dataset in SimianViewerMultExpComp viewer", getIcon(), [this, getInstance, datasets](PluginTriggerAction& pluginTriggerAction) -> void {
					for (auto dataset : datasets)
						getInstance()->loadData(Datasets({ dataset }));
					});

				pluginTriggerActions << pluginTriggerAction;
			}
		}
	}

	return pluginTriggerActions;
}


//bool SimianViewerMultExpCompViewerPluginFactory::hasHelp()
//{
//	return true;
//}