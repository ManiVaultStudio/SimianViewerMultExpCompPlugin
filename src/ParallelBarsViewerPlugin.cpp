#include "ParallelBarsViewerPlugin.h"

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

Q_PLUGIN_METADATA(IID "nl.tudelft.ParallelBarsViewerPlugin")

using namespace hdps;

// =============================================================================
// View
// =============================================================================

ParallelBarsViewerPlugin::ParallelBarsViewerPlugin(const PluginFactory* factory) :
	ViewPlugin(factory),
	_ParallelBars_viewer(),
	_ParallelBarsOptionsAction(*this, _core)
{
	setSerializationName("ParallelBarsViewer");
	//_ParallelBars_viewer = new ParallelBarsViewerWidget();


	getVisibleAction().setConnectionPermissionsFlag(ConnectionPermissionFlag::All);
	if (getFactory()->getNumberOfInstances() == 0)
	{
		getVisibleAction().publish("ParallelBars::PluginVisibility");
	}


	//connect(&getVisibleAction(), &ToggleAction::toggled, this, [](bool toggled) -> void {
	//	qDebug() << __FUNCTION__ << "toggled changed to" << toggled;
	//	});
}

ParallelBarsViewerPlugin::~ParallelBarsViewerPlugin()
{
}

void ParallelBarsViewerPlugin::init()
{
	connect(&_ParallelBars_viewer, &ParallelBarsViewerWidget::widgetInitialized, &_ParallelBarsOptionsAction, &ParallelBarsOptionsAction::initLoader);
	_ParallelBars_viewer.setPage(":/ParallelBars_viewer/ParallelBars_viewer.html", "qrc:/ParallelBars_viewer/");
	_ParallelBars_viewer.setContentsMargins(0, 0, 0, 0);
	_ParallelBars_viewer.layout()->setContentsMargins(0, 0, 0, 0);
	//_ParallelBarsOptionsAction = new ParallelBarsOptionsAction(*this, _core);
	connect(&_ParallelBars_viewer, &ParallelBarsViewerWidget::passSelectionSpecies1ToQt, this, &ParallelBarsViewerPlugin::publishSelectionSpecies1);

	connect(&_ParallelBars_viewer, &ParallelBarsViewerWidget::passSelectionSpecies2ToQt, this, &ParallelBarsViewerPlugin::publishSelectionSpecies2);

	connect(&_ParallelBars_viewer, &ParallelBarsViewerWidget::crossspeciesclusterSelection, this, &ParallelBarsViewerPlugin::clusterSelection);

	_eventListener.addSupportedEventType(static_cast<std::uint32_t>(EventType::DatasetDataSelectionChanged));
	_eventListener.registerDataEventByType(ClusterType, std::bind(&ParallelBarsViewerPlugin::onDataEvent, this, std::placeholders::_1));

	auto topToolbarWidget = new QWidget();
	auto topToolbarLayout = new QHBoxLayout();
	topToolbarLayout->setContentsMargins(0, 0, 0, 0);
	topToolbarLayout->setSpacing(0);

	auto ClusterDataset1SelectionWidget = _ParallelBarsOptionsAction.getdeStatsDataset1SelectionAction().createCollapsedWidget(&getWidget());
	//ClusterDataset1SelectionWidget->setMaximumWidth(280);
	//topToolbarLayout->addWidget(ClusterDataset1SelectionWidget);
	//auto humancompTempWidget = _ParallelBarsOptionsAction.getHumancomparisonAction().createWidget(&getWidget());
	//humancompTempWidget->setFixedWidth(270);
	//topToolbarLayout->addWidget(humancompTempWidget);
	topToolbarLayout->addWidget(&_ParallelBarsOptionsAction.getRadioButtonforAllSpeciesGeneExpression());
	topToolbarLayout->addWidget(&_ParallelBarsOptionsAction.getRadioButtonforHumandifferentialExpression());
	//topToolbarLayout->addSpacing(10);
	//topToolbarLayout->addWidget(_ParallelBarsOptionsAction.getHumancomparisonAbsoluteValuesAction().createWidget(&getWidget()));
	//auto ClusterDataset2SelectionWidget = _ParallelBarsOptionsAction.getdeStatsDataset2SelectionAction().createCollapsedWidget(&getWidget());
	////ClusterDataset2SelectionWidget->setMaximumWidth(280);
	//topToolbarLayout->addWidget(ClusterDataset2SelectionWidget);

	//topToolbarLayout->addWidget(_ParallelBarsOptionsAction.getScreenshotAction().createWidget(&getWidget()));
	//topToolbarLayout->addWidget(_ParallelBarsOptionsAction.getHelpAction().createWidget(&getWidget()));

	topToolbarLayout->addStretch(0);
	topToolbarWidget->setAutoFillBackground(true);
	topToolbarWidget->setLayout(topToolbarLayout);
	auto layout = new QVBoxLayout();
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);
	layout->addWidget(topToolbarWidget);
	layout->addWidget(&_ParallelBars_viewer, 1);
	getWidget().setLayout(layout);


}

void ParallelBarsViewerPlugin::onDataEvent(hdps::DatasetEvent* dataEvent)
{
	if (dataEvent->getType() == hdps::EventType::DatasetDataSelectionChanged)
	{


	}
}

void ParallelBarsViewerPlugin::publishSelectionSpecies1(std::string clusterName)
{

	//qDebug() << QString::fromStdString(selectedIDs);
	//_ParallelBarsOptionsAction.getCrossSpecies1HeatMapCellAction().setCurrentText("");
	//_ParallelBarsOptionsAction.getCrossSpecies1HeatMapCellAction().setCurrentText(QString::fromStdString(clusterName));
	auto dataset = _ParallelBarsOptionsAction.getdeStatsDataset1SelectAction().getCurrentDataset();
	const auto candidateDataset = _core->requestDataset<Clusters>(dataset.getDatasetId());
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

void ParallelBarsViewerPlugin::clusterSelection(std::string clusterName)
{
	_ParallelBarsOptionsAction.getSelectedCrossspeciesclusterFlag() = false;
	if (clusterName == "")
	{
		QString::fromStdString(clusterName);
		_ParallelBarsOptionsAction.getSelectedCrossspeciescluster().setString("");
	}
	else
	{
		//qDebug() << QString::fromStdString(clusterName);
		_ParallelBarsOptionsAction.getSelectedCrossspeciescluster().setString(QString::fromStdString(clusterName));
	}


}


void ParallelBarsViewerPlugin::publishSelectionSpecies2(std::string clusterName)
{

	//qDebug() << QString::fromStdString(selectedIDs);
	//_ParallelBarsOptionsAction.getCrossSpecies2HeatMapCellAction().setCurrentText("");
	//_ParallelBarsOptionsAction.getCrossSpecies2HeatMapCellAction().setCurrentText(QString::fromStdString(clusterName));
	auto dataset = _ParallelBarsOptionsAction.getdeStatsDataset2SelectAction().getCurrentDataset();
	const auto candidateDataset = _core->requestDataset<Clusters>(dataset.getDatasetId());
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

void ParallelBarsViewerPlugin::fromVariantMap(const QVariantMap& variantMap)
{
	ViewPlugin::fromVariantMap(variantMap);

	_ParallelBarsOptionsAction.fromParentVariantMap(variantMap);
}

QVariantMap ParallelBarsViewerPlugin::toVariantMap() const
{
	QVariantMap variantMap = ViewPlugin::toVariantMap();

	_ParallelBarsOptionsAction.insertIntoVariantMap(variantMap);

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

//ParallelBarsViewerPluginFactory::ParallelBarsViewerPluginFactory() :
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


QIcon ParallelBarsViewerPluginFactory::getIcon(const QColor& color /*= Qt::black*/) const
{
	return Application::getIconFont("FontAwesome").getIcon("chart-bar", color);
}

ViewPlugin* ParallelBarsViewerPluginFactory::produce()
{
	return new ParallelBarsViewerPlugin(this);
}

hdps::DataTypes ParallelBarsViewerPluginFactory::supportedDataTypes() const
{
	DataTypes supportedTypes;
	return supportedTypes;
}


hdps::gui::PluginTriggerActions ParallelBarsViewerPluginFactory::getPluginTriggerActions(const hdps::Datasets& datasets) const
{
	PluginTriggerActions pluginTriggerActions;

	const auto getInstance = [this]() -> ParallelBarsViewerPlugin* {
		return dynamic_cast<ParallelBarsViewerPlugin*>(plugins().requestViewPlugin(getKind()));
	};

	const auto numberOfDatasets = datasets.count();

	if (PluginFactory::areAllDatasetsOfTheSameType(datasets, PointType)) {
		if (numberOfDatasets >= 1) {
			if (datasets.first()->getDataType() == PointType) {
				auto pluginTriggerAction = new PluginTriggerAction(const_cast<ParallelBarsViewerPluginFactory*>(this), this, "ParallelBars viewer", "Load dataset in ParallelBars viewer", getIcon(), [this, getInstance, datasets](PluginTriggerAction& pluginTriggerAction) -> void {
					for (auto dataset : datasets)
						getInstance()->loadData(Datasets({ dataset }));
					});

				pluginTriggerActions << pluginTriggerAction;
			}
		}
	}

	return pluginTriggerActions;
}


//bool ParallelBarsViewerPluginFactory::hasHelp()
//{
//	return true;
//}