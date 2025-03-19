#pragma once

#include <ViewPlugin.h>
#include "Dataset.h"
#include "SimianViewerMultExpCompOptionsAction.h"
#include "SimianViewerMultExpCompWidget.h"
#include "widgets/DropWidget.h"
#include <QBoxLayout>
#include <QLabel>
#include <QStackedWidget>
#include <QList>

using namespace mv::plugin;
using namespace mv::util;

class Points;
//class SimianViewerMultExpCompWidget;
//class SimianViewerMultExpCompOptionsAction;

// =============================================================================
// View
// =============================================================================

/**
 * SimianViewerMultExpComp Plugin
 */
class SimianViewerMultExpCompPlugin : public ViewPlugin
{
	Q_OBJECT

public:
	SimianViewerMultExpCompPlugin(const PluginFactory* factory);
	~SimianViewerMultExpCompPlugin(void) override;

	void init() override;

	void onDataEvent(mv::DatasetEvent* dataEvent);

	mv::CoreInterface* getCore() { return _core; }

	SimianViewerMultExpCompWidget& getBarChartWidget() { return _SimianViewerMultExpComp_viewer; }
	SimianViewerMultExpCompOptionsAction& getSimianViewerMultExpCompOptionsAction() { return _SimianViewerMultExpCompOptionsAction; }

public: // Serialization

	/**
	 * Load widget action from variant map
	 * @param Variant map representation of the widget action
	 */
	void fromVariantMap(const QVariantMap& variantMap) override;

	/**
	 * Save widget action to variant map
	 * @return Variant map representation of the widget action
	 */
	QVariantMap toVariantMap() const override;

protected slots:


private:

	void publishSelectionSpecies1(std::string selectedIDs);
	void publishSelectionSpecies2(std::string selectedIDs);
	void clusterSelection(std::string selectedIDs);

	SimianViewerMultExpCompWidget _SimianViewerMultExpComp_viewer;
	SimianViewerMultExpCompOptionsAction _SimianViewerMultExpCompOptionsAction;
	/** SimianViewerMultExpComp widget displaying cluster data */

	mv::EventListener     _eventListener;
};

// =============================================================================
// Factory
// =============================================================================

class SimianViewerMultExpCompPluginFactory : public ViewPluginFactory
{
	Q_INTERFACES(mv::plugin::ViewPluginFactory mv::plugin::PluginFactory)
		Q_OBJECT
		Q_PLUGIN_METADATA(IID   "nl.tudelft.SimianViewerMultExpCompPlugin"
			FILE  "SimianViewerMultExpCompPlugin.json")

public:
	SimianViewerMultExpCompPluginFactory(void) {}
	~SimianViewerMultExpCompPluginFactory(void) override {}

	ViewPlugin* produce() override;

	mv::DataTypes supportedDataTypes() const override;

	/**
	 * Get plugin trigger actions given \p datasets
	 * @param datasets Vector of input datasets
	 * @return Vector of plugin trigger actions
	 */
	PluginTriggerActions getPluginTriggerActions(const mv::Datasets& datasets) const override;
	//public: // Help
	//
	///**
	// * Get whether the plugin has help information or not
	// * @return Boolean determining whether the plugin has help information or not
	// */
	//	bool hasHelp() override;
};