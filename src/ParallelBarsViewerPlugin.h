#pragma once

#include <ViewPlugin.h>
#include "Dataset.h"
#include "ParallelBarsOptionsAction.h"
#include "ParallelBarsViewerWidget.h"
#include "widgets/DropWidget.h"
#include <QBoxLayout>
#include <QLabel>
#include <QStackedWidget>
#include <QList>

using namespace mv::plugin;
using namespace mv::util;

class Points;
//class ParallelBarsViewerWidget;
//class ParallelBarsOptionsAction;

// =============================================================================
// View
// =============================================================================

/**
 * ParallelBarsViewer Plugin
 */
class ParallelBarsViewerPlugin : public ViewPlugin
{
	Q_OBJECT

public:
	ParallelBarsViewerPlugin(const PluginFactory* factory);
	~ParallelBarsViewerPlugin(void) override;

	void init() override;

	void onDataEvent(mv::DatasetEvent* dataEvent);

	mv::CoreInterface* getCore() { return _core; }

	ParallelBarsViewerWidget& getBarChartWidget() { return _ParallelBars_viewer; }
	ParallelBarsOptionsAction& getParallelBarsOptionsAction() { return _ParallelBarsOptionsAction; }

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

	ParallelBarsViewerWidget _ParallelBars_viewer;
	ParallelBarsOptionsAction _ParallelBarsOptionsAction;
	/** ParallelBarsViewer widget displaying cluster data */

	mv::EventListener     _eventListener;
};

// =============================================================================
// Factory
// =============================================================================

class ParallelBarsViewerPluginFactory : public ViewPluginFactory
{
	Q_INTERFACES(mv::plugin::ViewPluginFactory mv::plugin::PluginFactory)
		Q_OBJECT
		Q_PLUGIN_METADATA(IID   "nl.tudelft.ParallelBarsViewerPlugin"
			FILE  "ParallelBarsViewerPlugin.json")

public:
	ParallelBarsViewerPluginFactory(void) {}
	~ParallelBarsViewerPluginFactory(void) override {}

	/**
	 * Get plugin icon
	 * @param color Icon color for flat (font) icons
	 * @return Icon
	 */
	QIcon getIcon(const QColor& color = Qt::black) const override;

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