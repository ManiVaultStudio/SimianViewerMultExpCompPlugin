#pragma once

#include <actions/WidgetAction.h>
#include <actions/IntegralAction.h>
#include <actions/OptionAction.h>
#include <actions/OptionsAction.h>
#include <actions/ToggleAction.h>
#include "actions/DatasetPickerAction.h"
#include "PointData/PointData.h"
#include "ClusterData/ClusterData.h"
#include "event/EventListener.h"
#include "actions/Actions.h"
#include "Plugin.h"
#include "DataHierarchyItem.h"
#include "Set.h"
#include <AnalysisPlugin.h>
#include <memory>
#include "SimianViewerMultExpCompWidget.h"
#include <algorithm>    
#include <QDebug>
#include <QLabel>
#include <QComboBox>
#include <QGroupBox>
#include <QPushButton>
#include <QGridLayout>
#include <QFormLayout>
#include <QString>
#include <string>
#include<QRadioButton>
//#include "SimianViewerMultExpCompPlugin.h"
#include <event/Event.h>
#include <QDebug>
#include <QLabel>
#include <string>

using namespace mv::gui;
class QMenu;
class SimianViewerMultExpCompPlugin;
//class SimianViewerMultExpCompWidget;
class FetchMetaData;
namespace mv
{
	class CoreInterface;
}

struct speciesStorage
{
	std::string clusterName;
	std::string humandeStatsCount;
	std::string chimpdeStatsCount;
	std::string gorilladeStatsCount;
	std::string rhesusdeStatsCount;
	std::string marmosetdeStatsCount;
	std::string deStatsColor;
};

class SimianViewerMultExpCompOptionsAction : public WidgetAction
{
public:
	class deStatsDataset1SelectionAction : public WidgetAction
	{
	protected:
		class Widget : public mv::gui::WidgetActionWidget {
		public:
			Widget(QWidget* parent, deStatsDataset1SelectionAction* deStatsDataset1SelectAction);

			friend class deStatsDataset1SelectionAction;
		};

		QWidget* getWidget(QWidget* parent, const std::int32_t& widgetFlags) override {
			return new deStatsDataset1SelectionAction::Widget(parent, this);
		};

	public:
		deStatsDataset1SelectionAction(SimianViewerMultExpCompOptionsAction& SimianViewerMultExpCompOptionsAction);

	protected:
		SimianViewerMultExpCompOptionsAction& _SimianViewerMultExpCompOptionsAction;

		friend class SimianViewerMultExpCompOptionsAction;
	};



public:
	void onDataEvent(mv::DatasetEvent* dataEvent);
protected:

	class Widget : public mv::gui::WidgetActionWidget {
	public:
		Widget(QWidget* parent, SimianViewerMultExpCompOptionsAction* SimianViewerMultExpCompOptionsAction);
	};

	QWidget* getWidget(QWidget* parent, const std::int32_t& widgetFlags) override {
		return new SimianViewerMultExpCompOptionsAction::Widget(parent, this);
	};

public:
	SimianViewerMultExpCompOptionsAction(SimianViewerMultExpCompPlugin& SimianViewerMultExpCompPlugin, mv::CoreInterface* core);

private:
	void updateData();
	void extractGeneDimensions(std::vector<QString>* geneNames, std::vector<float>* geneColumn, mv::Dataset<Points> deStatsDataset);
protected:
	void updateDatasetPickerAction();

public: // Action getters


	deStatsDataset1SelectionAction& getdeStatsDataset1SelectionAction() { return _deStatsDataset1SelectionAction; }
	//TriggerAction& getScreenshotAction() { return _screenshotAction; }
	//TriggerAction& getHelpAction() { return _helpAction; }
	StringAction& getGeneName() { return _geneNameAction; }
	DatasetPickerAction& getdeStatsDataset1SelectAction() { return _deStatsDataset1Action; }
	DatasetPickerAction& getdeStatsDataset2SelectAction() { return _deStatsDataset2Action; }
	//OptionAction& getCrossSpecies1HeatMapCellAction() { return _crossSpecies1HeatMapCellAction; }
	//OptionAction& getCrossSpecies2HeatMapCellAction() { return _crossSpecies2HeatMapCellAction; }
	StringAction& getSelectedCrossspeciescluster() { return _selectedCrossspeciescluster; }
	StringAction& getSpecies1Name() { return _species1Name; }
	StringAction& getSpecies2Name() { return _species2Name; }
	bool& getSelectedCrossspeciesclusterFlag() { return _selectedCrossspeciesclusterFlag; }
	OptionAction& getNeighborhoodAction() { return _neighborhoodAction; }
	ColorAction& getSelectionColorAction() { return _selectionColorAction; }
	//ToggleAction& getHumancomparisonAction() { return _humancomparisonAction; };
	//ToggleAction& getHumancomparisonAbsoluteValuesAction() { return _humancomparisonAbsoluteValuesAction; };
	QRadioButton& getRadioButtonforAllSpeciesGeneExpression() { return _radioButtonforAllSpeciesGeneExpression; };
	QRadioButton& getRadioButtonforHumandifferentialExpression() { return _radioButtonforHumandifferentialExpression; };
public:
	void initLoader();

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

protected:
	SimianViewerMultExpCompPlugin& _SimianViewerMultExpCompPlugin;
	mv::CoreInterface* _core;
	FetchMetaData* _metaData;
	std::vector<std::vector<std::string>>                _SimianViewerMultExpCompData;
	deStatsDataset1SelectionAction               _deStatsDataset1SelectionAction;
	OptionAction                 _neighborhoodAction;
	//OptionAction                 _crossSpecies1HeatMapCellAction;
	//OptionAction                 _crossSpecies2HeatMapCellAction;
	//Actions
	//TriggerAction                _screenshotAction;
	//TriggerAction                _helpAction;
	std::map<std::string, speciesStorage> _deStatsDataStorage;
	DatasetPickerAction _AllcrossSpeciesDatasets;
	mv::Dataset<Points> _humanDEDataset;
	mv::Dataset<Points>  _chimpDEDataset;
	mv::Dataset<Points>  _gorillaDEDataset;
	mv::Dataset<Points>  _rhesusDEDataset;
	mv::Dataset<Points>  _marmosetDEDataset;
	DatasetPickerAction                 _deStatsDataset1Action;
	DatasetPickerAction                 _deStatsDataset2Action;
	mv::Dataset<Clusters> _humanClusterDataset;
	mv::Dataset<Clusters>  _chimpClusterDataset;
	mv::Dataset<Clusters>  _gorillaClusterDataset;
	mv::Dataset<Clusters>  _rhesusClusterDataset;
	mv::Dataset<Clusters>  _marmosetClusterDataset;
	StringAction                  _selectedCrossspeciescluster;
	StringAction                  _species1Name;
	StringAction                  _species2Name;
	bool _selectedCrossspeciesclusterFlag;
	ColorAction             _selectionColorAction;
	//std::vector<std::string> glia_Neighborhood = { "Astro_1", "Endo_1", "Micro-PVM_1", "OPC_1", "OPC_2", "Oligo_1", "VLMC_1" };
	//std::vector<std::string> it_types_Neighborhood = { "L2/3 IT_1", "L2/3 IT_2", "L2/3 IT_3", "L4 IT_1", "L4 IT_2", "L5 IT_1", "L5 IT_2", "L6 IT Car3_1", "L6 IT Car3_2", "L6 IT_1" };
	//std::vector<std::string> l5et_l56np_l6ct_l6b_Neighborhood = { "L5 ET_1", "L5 ET_2", "L5/6 NP_1", "L5/6 NP_2", "L6 CT_1", "L6 CT_2", "L6b_1", "L6b_2", "L6b_3" };
	//std::vector<std::string> lamp5_sncg_vip_Neighborhood = { "Lamp5_1", "Lamp5_2", "Lamp5_Lhx6_1", "Pax6_1", "Pax6_2", "Sncg_1", "Sncg_2", "Sncg_3", "Vip_1", "Vip_2", "Vip_3", "Vip_4", "Vip_5", "Vip_6", "Vip_7", "Vip_8" };
	//std::vector<std::string> sst_sst_chodl_pvalb_Neighborhood = { "Chandelier_1", "Pvalb_1", "Pvalb_2", "Pvalb_3", "Pvalb_4", "Sst Chodl_1", "Sst_1", "Sst_2", "Sst_3", "Sst_4", "Sst_5", "Sst_6", "Sst_7", "Sst_8", "Sst_9" };
	StringAction _geneNameAction;
	//ToggleAction _humancomparisonAction;
	//ToggleAction _humancomparisonAbsoluteValuesAction;
	mv::EventListener     _eventListener;
	QRadioButton _radioButtonforAllSpeciesGeneExpression;
	QRadioButton _radioButtonforHumandifferentialExpression;
	friend class ChannelAction;
};