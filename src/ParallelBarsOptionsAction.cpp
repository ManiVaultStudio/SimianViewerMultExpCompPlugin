#include "ParallelBarsOptionsAction.h"
#include "ParallelBarsViewerPlugin.h"
#include<string>  
#include <QFileDialog>
#include <QPageLayout>
#include <QWebEngineView>
using namespace hdps;
using namespace hdps::gui;

ParallelBarsOptionsAction::ParallelBarsOptionsAction(ParallelBarsViewerPlugin& ParallelBarsViewerPlugin, hdps::CoreInterface* core) :
	WidgetAction(&ParallelBarsViewerPlugin,"ParallelBarsViewerPlugin"),
	_ParallelBarsViewerPlugin(ParallelBarsViewerPlugin),
	_core(core),
	_geneNameAction(this, "SelectedID"),
	_deStatsDataset1Action(this, "DE Dataset1"),
	_deStatsDataset2Action(this, "DE Dataset2"),
	//_helpAction(this, "Help"),
	//_screenshotAction(this, "Screenshot"),
	_deStatsDataset1SelectionAction(*this),
	_selectedCrossspeciescluster(this, "Selected CrossSpecies Cluster"),
	_neighborhoodAction(this, "Neighborhood"),
	_species1Name(this, "Species1Name"),
	_species2Name(this, "Species2Name"),
	_selectionColorAction(this, "Selection color"),
	//_humancomparisonAction(this, "Gene expression: all species"),
	//_humancomparisonAbsoluteValuesAction(this, "Absolute values ")//,
	//_radioButtonforHumandifferentialExpression(this, "Absolute values "),
	//_radioButtonforAllSpeciesGeneExpression(this, "Radio Button All Species Gene Expression")
	//,
	//_crossSpecies1HeatMapCellAction(this, "Link cross-species1 heatmap cell"),
	//_crossSpecies2HeatMapCellAction(this, "Link cross-species2 heatmap cell")
	_AllcrossSpeciesDatasets(this, "All cross-species datasets")
{
	setSerializationName("ParallelBarsPyramidSettings");
	_species1Name.setSerializationName("Species1Name");
	_species2Name.setSerializationName("Species2Name");
	//_humancomparisonAbsoluteValuesAction.setVisible(false);
	_radioButtonforAllSpeciesGeneExpression.setText("Gene expression: all species");
	_radioButtonforHumandifferentialExpression.setText("Differential expression: human vs other species");
	_radioButtonforAllSpeciesGeneExpression.setChecked(true);
	_radioButtonforHumandifferentialExpression.setChecked(false);
	_geneNameAction.setSerializationName("Gene");
	_deStatsDataset1Action.setSerializationName("Species1(X-axis)");
	_deStatsDataset2Action.setSerializationName("Species2(Y-axis)");
	_selectedCrossspeciescluster.setSerializationName("Selected CrossSpecies Cluster"); 
	_neighborhoodAction.setSerializationName("Neighborhood");
	_selectionColorAction.setSerializationName("SelectionColor");
	_eventListener.addSupportedEventType(static_cast<std::uint32_t>(EventType::DatasetAdded));
	_eventListener.addSupportedEventType(static_cast<std::uint32_t>(EventType::DatasetRemoved));
	_eventListener.addSupportedEventType(static_cast<std::uint32_t>(EventType::DatasetChildAdded));
	_eventListener.addSupportedEventType(static_cast<std::uint32_t>(EventType::DatasetChildRemoved));
	_eventListener.addSupportedEventType(static_cast<std::uint32_t>(EventType::DatasetDataChanged));
	//_eventListener.addSupportedEventType(static_cast<std::uint32_t>(EventType::DataGuiNameChanged));
	_eventListener.registerDataEventByType(PointType, std::bind(&ParallelBarsOptionsAction::onDataEvent, this, std::placeholders::_1));
	_AllcrossSpeciesDatasets.setShowFullPathName(false);
	_AllcrossSpeciesDatasets.setDatasetsFilterFunction([this](const hdps::Datasets& datasets) ->hdps::Datasets {
		Datasets clusterDatasets;

		for (auto dataset : datasets)
			if (dataset->getDataType() == ClusterType)
			{
				std::string str1 = dataset->getGuiName().toStdString();
				std::string str2 = "cross_species_cluster";
				if (strstr(str1.c_str(), str2.c_str()))
				{
					clusterDatasets << dataset;
				}
			}
		return clusterDatasets;
		});
	_deStatsDataset1Action.setShowFullPathName(false);
	_deStatsDataset1Action.setDatasetsFilterFunction([this](const hdps::Datasets& datasets) ->hdps::Datasets {
		Datasets pointDatasets;

		for (auto dataset : datasets)
		{
			if (dataset->getDataType() == PointType)
			{
				std::string str1 = dataset->getGuiName().toStdString();
				std::string str2 = "DE_Statistics";
				if (strstr(str1.c_str(), str2.c_str()))
				{
					pointDatasets << dataset;
				}
			}

		}

		return pointDatasets;
		});
	_deStatsDataset2Action.setShowFullPathName(false);
		_deStatsDataset2Action.setDatasetsFilterFunction([this](const hdps::Datasets& datasets) ->hdps::Datasets {
			Datasets pointDatasets;

			for (auto dataset : datasets)
			{
				if (dataset->getDataType() == PointType)
				{
					std::string str1 = dataset->getGuiName().toStdString();
					std::string str2 = "DE_Statistics";
					if (strstr(str1.c_str(), str2.c_str()))
					{
						pointDatasets << dataset;
					}
				}

			}

			return pointDatasets;
			});

	//_barSettingsAction.setEnabled(false);
	//_deStatsDataset2SelectionAction.setEnabled(false);
	_geneNameAction.setString("A1BG");
	_species1Name.setString("Species1");
	_species2Name.setString("Species2");
	_selectedCrossspeciesclusterFlag = true;
	_selectedCrossspeciescluster.setString("");
	_neighborhoodAction.setDefaultWidgetFlags(OptionAction::ComboBox);
	_neighborhoodAction.initialize(QStringList({ "Non-neuronal cells","IT-projecting excitatory","Non-IT-projecting excitatory","CGE-derived inhibitory","MGE-derived inhibitory" }), "CGE-derived inhibitory");
	//_humancomparisonAction.setDefaultWidgetFlags(ToggleAction::PushButton);
	//_humancomparisonAction.initialize(false, false);
	//_humancomparisonAction.setText("Gene expression: all species");
	//_humancomparisonAction.setHighlighted(false);
	//_humancomparisonAbsoluteValuesAction.setDefaultWidgetFlags(ToggleAction::CheckBox);
	//_humancomparisonAbsoluteValuesAction.initialize(false, false);
	//_humancomparisonAbsoluteValuesAction.setVisible(false);
	//_helpAction.setDefaultWidgetFlags(TriggerAction::Icon);
	//_screenshotAction.setDefaultWidgetFlags(TriggerAction::Icon);
	//connect(&_helpAction, &TriggerAction::triggered, this, [this]() -> void {
	//	_ParallelBarsViewerPlugin.getTriggerHelpAction().trigger();
	//	});
	//_crossSpecies1HeatMapCellAction.setDefaultWidgetFlags(OptionAction::ComboBox);
	//_crossSpecies2HeatMapCellAction.setDefaultWidgetFlags(OptionAction::ComboBox);
	//_crossSpecies2HeatMapCellAction.initialize(QStringList({ " ","exc","glia","inh","Astro_2","Astro_3","Astro_4","Chandelier_1","Chandelier_2","Endo_1","Endo_2","L2/3 IT_1","L2/3 IT_2","L2/3 IT_3","L2/3 IT_4","L4 IT_1","L4 IT_2","L4 IT_3","L4 IT_4","L4 IT_5","L5 ET_1","L5 ET_2","L5 ET_3","L5 ET_4","L5 IT_1","L5 IT_2","L5 IT_3","L5/6 NP_1","L5/6 NP_2","L5/6 NP_3","L5/6 NP_4","L6 CT_1","L6 CT_2","L6 IT Car3_1","L6 IT Car3_2","L6 IT Car3_3","L6 IT_1","L6 IT_2","L6 IT_3","L6b_1","L6b_2","L6b_3","L6b_4","L6b_5","L6b_6","Lamp5_1","Lamp5_2","Lamp5_Lhx6_1","Lamp5_Lhx6_2","Micro-PVM_1","Micro-PVM_2","Micro-PVM_3","OPC_1","OPC_2","OPC_3","Oligo_1","Pax6_1","Pax6_2","Pax6_3","Pax6_4","Pvalb_1","Pvalb_2","Pvalb_3","Pvalb_4","Pvalb_5","Pvalb_6","Pvalb_7","Pvalb_8","Sncg_1","Sncg_2","Sncg_3","Sncg_4","Sncg_5","Sncg_6","Sncg_7","Sst Chodl_1","Sst Chodl_2","Sst Chodl_3","Sst_1","Sst_10","Sst_11","Sst_12","Sst_13","Sst_14","Sst_15","Sst_16","Sst_17","Sst_18","Sst_2","Sst_3","Sst_4","Sst_5","Sst_6","Sst_7","Sst_8","Sst_9","VLMC_1","VLMC_2","VLMC_3","VLMC_4","Vip_1","Vip_10","Vip_11","Vip_12","Vip_13","Vip_14","Vip_15","Vip_16","Vip_17","Vip_2","Vip_3","Vip_4","Vip_5","Vip_6","Vip_7","Vip_8","Vip_9","Astro_5","L2/3 IT_10","L2/3 IT_11","L2/3 IT_12","L2/3 IT_13","L2/3 IT_5","L2/3 IT_6","L2/3 IT_7","L2/3 IT_8","L2/3 IT_9","L5 IT_4","L5 IT_5","L5 IT_6","L5 IT_7","L5/6 NP_5","L5/6 NP_6","L6 CT_3","L6 CT_4","Lamp5_3","Lamp5_4","Lamp5_5","Lamp5_6","Oligo_2","Oligo_3","Oligo_4","Pvalb_10","Pvalb_11","Pvalb_12","Pvalb_13","Pvalb_14","Pvalb_15","Pvalb_9","Sncg_8","Sst_19","Sst_20","Sst_21","Sst_22","Sst_23","Sst_24","Sst_25","Sst_26","Vip_18","Vip_19","Vip_20","Vip_21","Vip_22","Vip_23","Endo_3","L4 IT_6","Micro-PVM_4","OPC_4","Sncg_9","L2/3 IT_14","L2/3 IT_15","L5/6 NP_7","L5/6 NP_8","L6 IT_4","Sncg_10","Glutamatergic","Non-Neuronal","Chandelier","Endo","L2/3 IT","L4 IT","L5 ET","L5 IT","L5/6 NP","L6 CT","L6 IT","L6 IT Car3","L6b","Lamp5","Lamp5_Lhx6","Micro-PVM","OPC","Oligo","Pax6","Pvalb","Sncg","Sst","Sst Chodl","VLMC","Vip","G19.32.002","G20.32.001","G20.32.002","H18.30.002","H19.30.001","H19.30.002","H200.1023","bi006","bi007","Q19.26.011","Q19.26.015","C19.32.002","C19.32.003","C19.32.004","C19.32.005","C19.32.006","C19.32.007","H18.30.001","bi005","Q19.26.010","C19.32.001","it_types","l5et_l56np_l6ct_l6b","lamp5_sncg_vip","sst_sst_chodl_pvalb","Astro_1","GABAergic","Astro","G19.32.001" }), " ", " ");
	//_crossSpecies1HeatMapCellAction.initialize(QStringList({ " ","exc","glia","inh","Astro_2","Astro_3","Astro_4","Chandelier_1","Chandelier_2","Endo_1","Endo_2","L2/3 IT_1","L2/3 IT_2","L2/3 IT_3","L2/3 IT_4","L4 IT_1","L4 IT_2","L4 IT_3","L4 IT_4","L4 IT_5","L5 ET_1","L5 ET_2","L5 ET_3","L5 ET_4","L5 IT_1","L5 IT_2","L5 IT_3","L5/6 NP_1","L5/6 NP_2","L5/6 NP_3","L5/6 NP_4","L6 CT_1","L6 CT_2","L6 IT Car3_1","L6 IT Car3_2","L6 IT Car3_3","L6 IT_1","L6 IT_2","L6 IT_3","L6b_1","L6b_2","L6b_3","L6b_4","L6b_5","L6b_6","Lamp5_1","Lamp5_2","Lamp5_Lhx6_1","Lamp5_Lhx6_2","Micro-PVM_1","Micro-PVM_2","Micro-PVM_3","OPC_1","OPC_2","OPC_3","Oligo_1","Pax6_1","Pax6_2","Pax6_3","Pax6_4","Pvalb_1","Pvalb_2","Pvalb_3","Pvalb_4","Pvalb_5","Pvalb_6","Pvalb_7","Pvalb_8","Sncg_1","Sncg_2","Sncg_3","Sncg_4","Sncg_5","Sncg_6","Sncg_7","Sst Chodl_1","Sst Chodl_2","Sst Chodl_3","Sst_1","Sst_10","Sst_11","Sst_12","Sst_13","Sst_14","Sst_15","Sst_16","Sst_17","Sst_18","Sst_2","Sst_3","Sst_4","Sst_5","Sst_6","Sst_7","Sst_8","Sst_9","VLMC_1","VLMC_2","VLMC_3","VLMC_4","Vip_1","Vip_10","Vip_11","Vip_12","Vip_13","Vip_14","Vip_15","Vip_16","Vip_17","Vip_2","Vip_3","Vip_4","Vip_5","Vip_6","Vip_7","Vip_8","Vip_9","Astro_5","L2/3 IT_10","L2/3 IT_11","L2/3 IT_12","L2/3 IT_13","L2/3 IT_5","L2/3 IT_6","L2/3 IT_7","L2/3 IT_8","L2/3 IT_9","L5 IT_4","L5 IT_5","L5 IT_6","L5 IT_7","L5/6 NP_5","L5/6 NP_6","L6 CT_3","L6 CT_4","Lamp5_3","Lamp5_4","Lamp5_5","Lamp5_6","Oligo_2","Oligo_3","Oligo_4","Pvalb_10","Pvalb_11","Pvalb_12","Pvalb_13","Pvalb_14","Pvalb_15","Pvalb_9","Sncg_8","Sst_19","Sst_20","Sst_21","Sst_22","Sst_23","Sst_24","Sst_25","Sst_26","Vip_18","Vip_19","Vip_20","Vip_21","Vip_22","Vip_23","Endo_3","L4 IT_6","Micro-PVM_4","OPC_4","Sncg_9","L2/3 IT_14","L2/3 IT_15","L5/6 NP_7","L5/6 NP_8","L6 IT_4","Sncg_10","Glutamatergic","Non-Neuronal","Chandelier","Endo","L2/3 IT","L4 IT","L5 ET","L5 IT","L5/6 NP","L6 CT","L6 IT","L6 IT Car3","L6b","Lamp5","Lamp5_Lhx6","Micro-PVM","OPC","Oligo","Pax6","Pvalb","Sncg","Sst","Sst Chodl","VLMC","Vip","G19.32.002","G20.32.001","G20.32.002","H18.30.002","H19.30.001","H19.30.002","H200.1023","bi006","bi007","Q19.26.011","Q19.26.015","C19.32.002","C19.32.003","C19.32.004","C19.32.005","C19.32.006","C19.32.007","H18.30.001","bi005","Q19.26.010","C19.32.001","it_types","l5et_l56np_l6ct_l6b","lamp5_sncg_vip","sst_sst_chodl_pvalb","Astro_1","GABAergic","Astro","G19.32.001" }), " ", " ");
	//_screenshotAction.setEnabled(false);
	//_helpAction.setIcon(Application::getIconFont("FontAwesome").getIcon("question"));
	//_screenshotAction.setIcon(Application::getIconFont("FontAwesome").getIcon("camera"));


	//_deStatsDataset1Action.setConnectionPermissionsFlag(ConnectionPermissionFlag::All);

	//_deStatsDataset2Action.setConnectionPermissionsFlag(ConnectionPermissionFlag::All);

	//_selectedCrossspeciescluster.setConnectionPermissionsFlag(ConnectionPermissionFlag::All);


	//_species1Name.setConnectionPermissionsFlag(ConnectionPermissionFlag::All);
	//_species1Name.connectToPublicActionByName("Cluster Differential Expression 1::DatasetName1");
	//_species2Name.setConnectionPermissionsFlag(ConnectionPermissionFlag::All);
	//_species2Name.connectToPublicActionByName("Cluster Differential Expression 1::DatasetName2");
	//_geneNameAction.setConnectionPermissionsFlag(ConnectionPermissionFlag::All);
	//_geneNameAction.connectToPublicActionByName("Cluster Differential Expression 1::LastSelectedId");

	//_neighborhoodAction.setConnectionPermissionsFlag(ConnectionPermissionFlag::All);

	//_geneNameAction.setConnectionPermissionsFlag(ConnectionPermissionFlag::All);
	//_geneNameAction.connectToPublicActionByName("Cluster Differential Expression 1::LastSelectedId");
	//_species1Name.setConnectionPermissionsFlag(ConnectionPermissionFlag::All);
	//_species1Name.connectToPublicActionByName("Cluster Differential Expression 1::DatasetName1");
	//_species2Name.setConnectionPermissionsFlag(ConnectionPermissionFlag::All);
	//_species2Name.connectToPublicActionByName("Cluster Differential Expression 1::DatasetName2");
	//_selectedCrossspeciescluster.setConnectionPermissionsFlag(ConnectionPermissionFlag::All);
	//_selectedCrossspeciescluster.connectToPublicActionByName("GlobalSelectedCrossspeciesCluster");

	//_deStatsDataset1Action.setConnectionPermissionsFlag(ConnectionPermissionFlag::All);
	//_deStatsDataset1Action.connectToPublicActionByName("Pop Pyramid:: DE Dataset1");
	//_deStatsDataset2Action.setConnectionPermissionsFlag(ConnectionPermissionFlag::All);
	//_deStatsDataset2Action.connectToPublicActionByName("Pop Pyramid:: DE Dataset2");

	//_selectionColorAction.setConnectionPermissionsFlag(ConnectionPermissionFlag::ConnectViaApi);
	//_selectionColorAction.connectToPublicActionByName("GlobalSelectionColor");
	const auto updateGeneName = [this]() -> void
	{
		updateData();


	};

	const auto updateSelectionColor = [this]() -> void
	{
		if (_selectionColorAction.getColor().isValid())
		{
			QColor color = _selectionColorAction.getColor();
			QString hexValueColor = "#" + QString::number(color.red(), 16).rightJustified(2, '0')
				+ QString::number(color.green(), 16).rightJustified(2, '0')
				+ QString::number(color.blue(), 16).rightJustified(2, '0');


			_ParallelBarsViewerPlugin.getBarChartWidget().updateSelectionColor(hexValueColor);

		}

	};

	connect(&_selectionColorAction, &ColorAction::colorChanged, this, updateSelectionColor);


	const auto updateSelectedCrossspeciescluster = [this]() -> void
	{
		if (_selectedCrossspeciesclusterFlag)
		{
			_ParallelBarsViewerPlugin.getBarChartWidget().setSelectedCrossspeciescluster(_selectedCrossspeciescluster.getString());
		}
		_selectedCrossspeciesclusterFlag = true;

	};


	//const auto generateScreenshot = [this]() -> void {


	//	QFileDialog saveFileDialog;

	//	saveFileDialog.setAcceptMode(QFileDialog::AcceptSave);
	//	saveFileDialog.setDirectory(QDir::home().absolutePath());

	//	saveFileDialog.selectFile("BarchartViewerScreenshot.pdf");
	//	saveFileDialog.setNameFilter(tr("PDF Files (*.pdf)"));

	//	QString fileName;
	//	if (saveFileDialog.exec())
	//	{
	//		fileName = saveFileDialog.selectedFiles().first();



	//		QPageLayout pl;
	//		QPageSize ps;
	//		//qDebug() << "height" << _simianViewerPlugin.getSimianViewerWidget()->height();

	//		int width = _ParallelBarsViewerPlugin.getBarChartWidget().width();
	//		int height = _ParallelBarsViewerPlugin.getBarChartWidget().height();
	//		int reducedWidth = static_cast<double>(width) / 100 * 75;
	//		int reducedHeight = static_cast<double>(height) / 100 * 78;
	//		//qDebug() << "width" << width;
	//		//qDebug() << "reduced width" << reducedWidth;
	//		//qDebug() << "height" << height;
	//		//qDebug() << "reduced height" << reducedHeight;
	//		ps = QPageSize(QSizeF(reducedWidth, reducedHeight), QPageSize::Point, QString(), QPageSize::ExactMatch);
	//		pl.setPageSize(ps);
	//		pl.setOrientation(QPageLayout::Portrait);


	//		_ParallelBarsViewerPlugin.getBarChartWidget().getPage()->printToPdf(fileName, pl);

	//	}
	//	//..getSimianViewerWidget()->getPage()->printToPdf(fileName, pl);

	//};

	const auto updateNeighborhood = [this]() -> void
	{
		if (_AllcrossSpeciesDatasets.getDatasets().size()!=0 && _neighborhoodAction.getCurrentText() != "" && _geneNameAction.getString() != "")
		{
			hdps::Datasets filteredDatasets;
			for (auto dataset : _AllcrossSpeciesDatasets.getDatasets())
			{
				std::string str1 = dataset->getGuiName().toStdString();
				std::string str2 = "_" + _neighborhoodAction.getCurrentText().toStdString() + "_cross_species_cluster";
				if (strstr(str1.c_str(), str2.c_str()))
				{
					filteredDatasets.append(dataset);
				}
			}

			if (filteredDatasets.size() != 0)
			{
				for (auto dataset : filteredDatasets)
				{
					std::string str1 = dataset->getGuiName().toStdString();
					std::string humanStr = "human";
					std::string chimpStr = "chimp";
					std::string gorillaStr = "gorilla";
					std::string rhesusStr = "rhesus";
					std::string marmosetStr = "marmoset";
					if (strstr(str1.c_str(), humanStr.c_str()))
					{
						const QString child_DE_Statistics_DatasetName = "DE_Statistics";
						_humanClusterDataset = dataset;
						QString DatasetGUID = "";
						const auto& childDatasets = _humanClusterDataset->getChildren({ PointType });
						for (qsizetype i = 0; i < childDatasets.size(); ++i)
						{
							if (childDatasets[i]->getGuiName() == child_DE_Statistics_DatasetName)
							{
								_humanDEDataset = childDatasets[i];
							}
						}

					}
					else if (strstr(str1.c_str(), chimpStr.c_str()))
					{

						const QString child_DE_Statistics_DatasetName = "DE_Statistics";
						_chimpClusterDataset = dataset;
						QString DatasetGUID = "";
						const auto& childDatasets = _chimpClusterDataset->getChildren({ PointType });
						for (qsizetype i = 0; i < childDatasets.size(); ++i)
						{
							if (childDatasets[i]->getGuiName() == child_DE_Statistics_DatasetName)
							{
								_chimpDEDataset = childDatasets[i];
							}
						}


					}
					else if (strstr(str1.c_str(), gorillaStr.c_str()))
					{
						const QString child_DE_Statistics_DatasetName = "DE_Statistics";
						_gorillaClusterDataset = dataset;
						QString DatasetGUID = "";
						const auto& childDatasets = _gorillaClusterDataset->getChildren({ PointType });
						for (qsizetype i = 0; i < childDatasets.size(); ++i)
						{
							if (childDatasets[i]->getGuiName() == child_DE_Statistics_DatasetName)
							{
								_gorillaDEDataset = childDatasets[i];
							}
						}

					}
					else if (strstr(str1.c_str(), rhesusStr.c_str()))
					{
						const QString child_DE_Statistics_DatasetName = "DE_Statistics";
						_rhesusClusterDataset = dataset;
						QString DatasetGUID = "";
						const auto& childDatasets = _rhesusClusterDataset->getChildren({ PointType });
						for (qsizetype i = 0; i < childDatasets.size(); ++i)
						{
							if (childDatasets[i]->getGuiName() == child_DE_Statistics_DatasetName)
							{
								_rhesusDEDataset = childDatasets[i];
							}
						}


					}
					else if (strstr(str1.c_str(), marmosetStr.c_str()))
					{
						const QString child_DE_Statistics_DatasetName = "DE_Statistics";
						_marmosetClusterDataset = dataset;
						QString DatasetGUID = "";
						const auto& childDatasets = _marmosetClusterDataset->getChildren({ PointType });
						for (qsizetype i = 0; i < childDatasets.size(); ++i)
						{
							if (childDatasets[i]->getGuiName() == child_DE_Statistics_DatasetName)
							{
								_marmosetDEDataset = childDatasets[i];
							}
						}
					}
				}
			}
		}
		updateData();

	};

	const auto updatedeStatsDataset1 = [this]() -> void
	{


	};

	const auto updatedeStatsDataset2 = [this]() -> void
	{

	};
	const auto updateSpecies1Name = [this]() -> void
	{
		if (_species1Name.getString() != "")
		{
			_ParallelBarsViewerPlugin.getBarChartWidget().setSpecies1(_species1Name.getString());
		}
	};
	const auto updateSpecies2Name = [this]() -> void
	{
		if (_species2Name.getString() != "")
		{
			_ParallelBarsViewerPlugin.getBarChartWidget().setSpecies2(_species2Name.getString());
		}
	};
	connect(&_species1Name, &StringAction::stringChanged, this, updateSpecies1Name);
	connect(&_species2Name, &StringAction::stringChanged, this, updateSpecies2Name);
	connect(&_deStatsDataset1Action, &DatasetPickerAction::datasetPicked, this, updatedeStatsDataset1);

	connect(&_deStatsDataset2Action, &DatasetPickerAction::datasetPicked, this, updatedeStatsDataset2);
	connect(&_neighborhoodAction, &OptionAction::currentIndexChanged, this, updateNeighborhood);
	connect(&_geneNameAction, &StringAction::stringChanged, this, updateGeneName);
	connect(&_selectedCrossspeciescluster, &StringAction::stringChanged, this, updateSelectedCrossspeciescluster);

	//const auto updateHumancomparisonAction = [this]() -> void
	//{
	//	updateData();
	//};

	//connect(&_humancomparisonAction, &ToggleAction::toggled, this, [this, updateHumancomparisonAction](const bool& toggled)
	//	{
	//		if (_humancomparisonAction.isChecked())
	//		{
	//			_humancomparisonAbsoluteValuesAction.setVisible(true);
	//			_humancomparisonAction.setText("Differential expression: human vs other species");
	//			//_humancomparisonAction.setHighlighted(false);
	//		}
	//		else
	//		{
	//			_humancomparisonAbsoluteValuesAction.setVisible(false);
	//			_humancomparisonAction.setText("Gene expression: all species");
	//			//_humancomparisonAction.setHighlighted(false);
	//		}
	//		
	//		updateHumancomparisonAction();
	//	});

	//	const auto updateHumancomparisonAbsoluteValuesAction = [this]() -> void
	//{
	//	updateData();
	//};

	//connect(&_humancomparisonAbsoluteValuesAction, &ToggleAction::toggled, this, [this, updateHumancomparisonAbsoluteValuesAction](const bool& toggled)
	//	{
	//		updateHumancomparisonAbsoluteValuesAction();
	//	});


	const auto RadioButtonAllSpeciesGeneExpressionUpdate = [this]() -> void
	{
		_radioButtonforHumandifferentialExpression.setChecked(false);
		updateData();
	};
	connect(&_radioButtonforAllSpeciesGeneExpression, &QRadioButton::toggled, this, RadioButtonAllSpeciesGeneExpressionUpdate);

	const auto RadioButtonHumandifferentialExpression = [this]() -> void
	{
		_radioButtonforAllSpeciesGeneExpression.setChecked(false);
		updateData();
	};
	connect(&_radioButtonforHumandifferentialExpression, &QRadioButton::toggled, this, RadioButtonHumandifferentialExpression);

	//updateDatasetPickerAction();
}


ParallelBarsOptionsAction::Widget::Widget(QWidget* parent, ParallelBarsOptionsAction* ParallelBarsOptionsAction) :
	WidgetActionWidget(parent, ParallelBarsOptionsAction)
{
}

void ParallelBarsOptionsAction::updateData()
{


	if (_marmosetDEDataset.getDatasetId()!="" && _rhesusDEDataset.getDatasetId() != "" && _gorillaDEDataset.getDatasetId() != "" && _chimpDEDataset.getDatasetId() != "" && _humanDEDataset.getDatasetId() != "" && _marmosetClusterDataset.getDatasetId() != "" && _rhesusClusterDataset.getDatasetId() != "" && _gorillaClusterDataset.getDatasetId()!="" && _chimpClusterDataset.getDatasetId() != "" && _humanClusterDataset.getDatasetId() != "")
	{

		//qDebug() << "+++++++++++++++++++++++++++++++++++++";
		//qDebug() << "Human";
		//qDebug() << _humanDEDataset->getGuiName();
		//qDebug() << _humanClusterDataset->getGuiName();
		//qDebug() << "+++++++++++++++++++++++++++++++++++++";
		//qDebug() << "Chimp";
		//qDebug() << _chimpDEDataset->getGuiName();
		//qDebug() << _chimpClusterDataset->getGuiName();
		//qDebug() << "+++++++++++++++++++++++++++++++++++++";
		//qDebug() << "Gorilla";
		//qDebug() << _gorillaDEDataset->getGuiName();
		//qDebug() << _gorillaClusterDataset->getGuiName();
		//qDebug() << "+++++++++++++++++++++++++++++++++++++";
		//qDebug() << "Rhesus";
		//qDebug() << _rhesusDEDataset->getGuiName();
		//qDebug() << _rhesusClusterDataset->getGuiName();
		//qDebug() << "+++++++++++++++++++++++++++++++++++++";
		//qDebug() << "Marmoset";
		//qDebug() << _marmosetDEDataset->getGuiName();
		//qDebug() << _marmosetClusterDataset->getGuiName();
		//qDebug() << "+++++++++++++++++++++++++++++++++++++";


		_deStatsDataStorage.clear();
		auto geneNamesHuman = _humanDEDataset->getDimensionNames();
		auto geneNamesChimp = _chimpDEDataset->getDimensionNames();
		auto geneNamesGorilla = _gorillaDEDataset->getDimensionNames();
		auto geneNamesRhesus = _rhesusDEDataset->getDimensionNames();
		auto geneNamesMarmoset = _marmosetDEDataset->getDimensionNames();
		std::vector<float> geneColumnHuman;
		std::vector<float> geneColumnChimp;
		std::vector<float> geneColumnGorilla;
		std::vector<float> geneColumnRhesus;
		std::vector<float> geneColumnMarmoset;
		extractGeneDimensions(&geneNamesHuman, &geneColumnHuman, _humanDEDataset);
		extractGeneDimensions(&geneNamesChimp, &geneColumnChimp, _chimpDEDataset);
		extractGeneDimensions(&geneNamesGorilla, &geneColumnGorilla, _gorillaDEDataset);
		extractGeneDimensions(&geneNamesRhesus, &geneColumnRhesus, _rhesusDEDataset);
		extractGeneDimensions(&geneNamesMarmoset, &geneColumnMarmoset, _marmosetDEDataset);
		auto clusterListHuman = _humanClusterDataset->getClusters();
		auto clusterListChimp = _chimpClusterDataset->getClusters();
		auto clusterListGorilla = _gorillaClusterDataset->getClusters();
		auto clusterListRhesus = _rhesusClusterDataset->getClusters();
		auto clusterListMarmoset = _marmosetClusterDataset->getClusters();

		//human
		for (int i = 0; i < clusterListHuman.size(); i++)
		{
			speciesStorage tempStore;
			tempStore.clusterName = clusterListHuman.at(i).getName().toStdString();
			tempStore.humandeStatsCount = std::to_string(geneColumnHuman.at(i));
			tempStore.chimpdeStatsCount = std::to_string(0);
			tempStore.gorilladeStatsCount = std::to_string(0);
			tempStore.rhesusdeStatsCount = std::to_string(0);
			tempStore.marmosetdeStatsCount = std::to_string(0);
			tempStore.deStatsColor = clusterListHuman.at(i).getColor().name().toStdString();
			_deStatsDataStorage.insert(std::pair<std::string, speciesStorage>(clusterListHuman.at(i).getName().toStdString(), tempStore));
		}
		//chimp
		for (int i = 0; i < clusterListChimp.size(); i++)
		{
			auto deStatsName = clusterListChimp.at(i).getName().toStdString();


			std::map<std::string, speciesStorage>::iterator it = _deStatsDataStorage.find(deStatsName);
			if (it != _deStatsDataStorage.end()) {
				it->second.chimpdeStatsCount = std::to_string(geneColumnChimp.at(i));
			}
			else
			{
				speciesStorage tempStore;
				tempStore.clusterName = clusterListChimp.at(i).getName().toStdString();
				tempStore.chimpdeStatsCount = std::to_string(geneColumnChimp.at(i));
				tempStore.humandeStatsCount = std::to_string(0);
				tempStore.gorilladeStatsCount = std::to_string(0);
				tempStore.rhesusdeStatsCount = std::to_string(0);
				tempStore.marmosetdeStatsCount = std::to_string(0);
				tempStore.deStatsColor = clusterListChimp.at(i).getColor().name().toStdString();
				_deStatsDataStorage.insert(std::pair<std::string, speciesStorage>(clusterListChimp.at(i).getName().toStdString(), tempStore));
			}

		}
		//gorilla
		for (int i = 0; i < clusterListGorilla.size(); i++)
		{
			auto deStatsName = clusterListGorilla.at(i).getName().toStdString();


			std::map<std::string, speciesStorage>::iterator it = _deStatsDataStorage.find(deStatsName);
			if (it != _deStatsDataStorage.end()) {
				it->second.gorilladeStatsCount = std::to_string(geneColumnGorilla.at(i));
			}
			else
			{
				speciesStorage tempStore;
				tempStore.clusterName = clusterListGorilla.at(i).getName().toStdString();
				tempStore.gorilladeStatsCount = std::to_string(geneColumnGorilla.at(i));
				tempStore.humandeStatsCount = std::to_string(0);
				tempStore.chimpdeStatsCount = std::to_string(0);
				tempStore.rhesusdeStatsCount = std::to_string(0);
				tempStore.marmosetdeStatsCount = std::to_string(0);
				tempStore.deStatsColor = clusterListGorilla.at(i).getColor().name().toStdString();
				_deStatsDataStorage.insert(std::pair<std::string, speciesStorage>(clusterListGorilla.at(i).getName().toStdString(), tempStore));
			}

		}
		//rhesus
		for (int i = 0; i < clusterListRhesus.size(); i++)
		{
			auto deStatsName = clusterListRhesus.at(i).getName().toStdString();


			std::map<std::string, speciesStorage>::iterator it = _deStatsDataStorage.find(deStatsName);
			if (it != _deStatsDataStorage.end()) {
				it->second.rhesusdeStatsCount = std::to_string(geneColumnRhesus.at(i));
			}
			else
			{
				speciesStorage tempStore;
				tempStore.clusterName = clusterListRhesus.at(i).getName().toStdString();
				tempStore.rhesusdeStatsCount = std::to_string(geneColumnRhesus.at(i));
				tempStore.humandeStatsCount = std::to_string(0);
				tempStore.gorilladeStatsCount = std::to_string(0);
				tempStore.chimpdeStatsCount = std::to_string(0);
				tempStore.marmosetdeStatsCount = std::to_string(0);
				tempStore.deStatsColor = clusterListRhesus.at(i).getColor().name().toStdString();
				_deStatsDataStorage.insert(std::pair<std::string, speciesStorage>(clusterListRhesus.at(i).getName().toStdString(), tempStore));
			}

		}
		//marmoset
		for (int i = 0; i < clusterListMarmoset.size(); i++)
		{
			auto deStatsName = clusterListMarmoset.at(i).getName().toStdString();


			std::map<std::string, speciesStorage>::iterator it = _deStatsDataStorage.find(deStatsName);
			if (it != _deStatsDataStorage.end()) {
				it->second.marmosetdeStatsCount = std::to_string(geneColumnMarmoset.at(i));
			}
			else
			{
				speciesStorage tempStore;
				tempStore.clusterName = clusterListMarmoset.at(i).getName().toStdString();
				tempStore.marmosetdeStatsCount = std::to_string(geneColumnMarmoset.at(i));
				tempStore.humandeStatsCount = std::to_string(0);
				tempStore.gorilladeStatsCount = std::to_string(0);
				tempStore.rhesusdeStatsCount = std::to_string(0);
				tempStore.chimpdeStatsCount = std::to_string(0);
				tempStore.deStatsColor = clusterListMarmoset.at(i).getColor().name().toStdString();
				_deStatsDataStorage.insert(std::pair<std::string, speciesStorage>(clusterListMarmoset.at(i).getName().toStdString(), tempStore));
			}

		}
		std::string humanComparisonflag = "hide";
		if (_radioButtonforHumandifferentialExpression.isChecked())
		{
			humanComparisonflag = "show";
		}
		else
		{
			humanComparisonflag = "hide";
		}

		std::string humanComparisonAbsoluteValuesflag = "hide";
		//if (_humancomparisonAbsoluteValuesAction.isChecked())
		//{
		//	humanComparisonAbsoluteValuesflag = "show";
		//}
		//else
		//{
		//	humanComparisonAbsoluteValuesflag = "hide";
		//}

		std::string jsonData = "[{";
		jsonData += '"';
		jsonData += "data";
		jsonData += '"';
		jsonData += ":";
		jsonData += '[';
		for (auto ittr = _deStatsDataStorage.rbegin(); ittr != _deStatsDataStorage.rend(); ++ittr)
		{
			//qDebug() << "\n++++++++++++++++++++++++++++++++++++++++++++++++++\n";
			//qDebug() << "clusterName: "<< QString::fromStdString(ittr->second.clusterName);
			//qDebug() << "species1deStatsCount: " << QString::fromStdString(ittr->second.species1deStatsCount);
			//qDebug() << "species2deStatsCount: " << QString::fromStdString(ittr->second.species2deStatsCount);
			//qDebug() << "deStatsColor: " << QString::fromStdString(ittr->second.deStatsColor);
			//qDebug() << "\n++++++++++++++++++++++++++++++++++++++++++++++++++\n";

			jsonData += "{";

			jsonData += '"';
			jsonData += "clusterName";
			jsonData += '"';
			jsonData += ":";
			jsonData += '"';
			jsonData += ittr->second.clusterName;
			jsonData += '"';
			jsonData += ",";
			jsonData += '"';
			jsonData += "species1ClusterCount";
			jsonData += '"';
			jsonData += ":";
			if (humanComparisonflag == "show")
			{
				float result = std::stof(ittr->second.humandeStatsCount) - std::stof(ittr->second.humandeStatsCount);

				//if (_humancomparisonAbsoluteValuesAction.isChecked())
				//{
				//	jsonData += std::to_string(std::abs(result));
				//}
				//else
				{
					jsonData += std::to_string(result);
				}

			}
			else {
				jsonData += ittr->second.humandeStatsCount;
			}

			jsonData += ",";
			jsonData += '"';
			jsonData += "species2ClusterCount";
			jsonData += '"';
			jsonData += ":";
			if (humanComparisonflag == "show")
			{
				float result = std::stof(ittr->second.humandeStatsCount) - std::stof(ittr->second.chimpdeStatsCount);
				//if (_humancomparisonAbsoluteValuesAction.isChecked())
				//{
				//	jsonData += std::to_string(std::abs(result));
				//}
				//else
				{
					jsonData += std::to_string(result);
				}
			}
			else {
				jsonData += ittr->second.chimpdeStatsCount;
			}


			jsonData += ",";
			jsonData += '"';
			jsonData += "species3ClusterCount";
			jsonData += '"';
			jsonData += ":";
			if (humanComparisonflag == "show")
			{
				float result = std::stof(ittr->second.humandeStatsCount) - std::stof(ittr->second.gorilladeStatsCount);
				//if (_humancomparisonAbsoluteValuesAction.isChecked())
				//{
				//	jsonData += std::to_string(std::abs(result));
				//}
				//else
				{
					jsonData += std::to_string(result);
				}
			}
			else {
				jsonData += ittr->second.gorilladeStatsCount;
			}


			jsonData += ",";
			jsonData += '"';
			jsonData += "species4ClusterCount";
			jsonData += '"';
			jsonData += ":";
			if (humanComparisonflag == "show")
			{
				float result = std::stof(ittr->second.humandeStatsCount) - std::stof(ittr->second.rhesusdeStatsCount);
				//if (_humancomparisonAbsoluteValuesAction.isChecked())
				//{
				//	jsonData += std::to_string(std::abs(result));
				//}
				//else
				{
					jsonData += std::to_string(result);
				}
			}
			else {
				jsonData += ittr->second.rhesusdeStatsCount;
			}


			jsonData += ",";
			jsonData += '"';
			jsonData += "species5ClusterCount";
			jsonData += '"';
			jsonData += ":";
			if (humanComparisonflag == "show")
			{
				float result = std::stof(ittr->second.humandeStatsCount) - std::stof(ittr->second.marmosetdeStatsCount);
				//if (_humancomparisonAbsoluteValuesAction.isChecked())
				//{
				///	jsonData += std::to_string(std::abs(result));
				//}
				//else
				{
					jsonData += std::to_string(result);
				}
			}
			else {
				jsonData += ittr->second.marmosetdeStatsCount;
			}
			jsonData += ",";
			jsonData += '"';
			jsonData += "clusterColor";
			jsonData += '"';
			jsonData += ":";
			jsonData += '"';
			jsonData += ittr->second.deStatsColor;
			jsonData += '"';
			jsonData += '}';
			jsonData += ",";
		}
		jsonData.pop_back();
		jsonData += "],";
		jsonData += '"';
		jsonData += "info";
		jsonData += '"';
		jsonData += ":[{";
		jsonData += '"';
		jsonData += "species1Name";
		jsonData += '"';
		jsonData += ":";
		jsonData += '"';
		jsonData += "human";
		jsonData += '"';
		jsonData += ",";
		jsonData += '"';
		jsonData += "species2Name";
		jsonData += '"';
		jsonData += ":";
		jsonData += '"';
		jsonData += "chimp";
		jsonData += '"';
		jsonData += ",";
		jsonData += '"';
		jsonData += "species3Name";
		jsonData += '"';
		jsonData += ":";
		jsonData += '"';
		jsonData += "gorilla";
		jsonData += '"';
		jsonData += ",";
		jsonData += '"';
		jsonData += "species4Name";
		jsonData += '"';
		jsonData += ":";
		jsonData += '"';
		jsonData += "rhesus";
		jsonData += '"';
		jsonData += ",";
		jsonData += '"';
		jsonData += "species5Name";
		jsonData += '"';
		jsonData += ":";
		jsonData += '"';
		jsonData += "marmoset";
		jsonData += '"';
		jsonData += ",";
		jsonData += '"';
		jsonData += "geneName";
		jsonData += '"';
		jsonData += ":";
		jsonData += '"';
		jsonData += _geneNameAction.getString().toStdString();
		jsonData += '"';
		jsonData += ",";
		jsonData += '"';
		jsonData += "humanComparisonflag";
		jsonData += '"';
		jsonData += ":";
		jsonData += '"';
		jsonData += humanComparisonflag;
		jsonData += '"';
		jsonData += ",";
		jsonData += '"';
		jsonData += "humanComparisonAbsoluteValuesflag";
		jsonData += '"';
		jsonData += ":";
		jsonData += '"';
		jsonData += humanComparisonAbsoluteValuesflag;
		jsonData += '"';
		jsonData += "}";
		jsonData += "]}]";
		_ParallelBarsViewerPlugin.getBarChartWidget().setData(jsonData);

	}
}



void ParallelBarsOptionsAction::extractGeneDimensions(std::vector<QString>* geneNames, std::vector<float>* geneColumn, hdps::Dataset<Points> deStatsDataset)

{
	auto it1 = std::find(geneNames->begin(), geneNames->end(), _geneNameAction.getString());
	if (it1 == geneNames->end())
	{
		deStatsDataset->extractDataForDimension(*geneColumn, 0);//name not in vector
		std::fill(geneColumn->begin(), geneColumn->end(), 0.0);
	}
	else
	{
		auto index = std::distance(geneNames->begin(), it1);
		deStatsDataset->extractDataForDimension(*geneColumn, index);
	}
}

//void ParallelBarsOptionsAction::updateDatasetPickerAction()
//{
//	_AllcrossSpeciesDatasets.clear();
//	auto ClusterDatasets = _core->requestAllDataSets(QVector<hdps::DataType> {ClusterType});
//	auto filteredClusterDatasets = ClusterDatasets;
//	for (auto dataset : ClusterDatasets)
//	{
//		std::string str1 = dataset->getGuiName().toStdString();
//		std::string str2 = "cross_species_cluster";
//		if (strstr(str1.c_str(), str2.c_str()))
//		{
//			_AllcrossSpeciesDatasets.append(dataset);
//		}
//		//else {
//		//	ClusterDatasets.removeOne(dataset);
//		//}
//	}
//
//	auto datasets = _core->requestAllDataSets(QVector<hdps::DataType> {PointType});
//	auto filteredDEStatsDatasets = datasets;
//	for (auto dataset : datasets)
//	{
//		std::string str1 = dataset->getGuiName().toStdString();
//		std::string str2 = "DE_Statistics";
//		if (strstr(str1.c_str(), str2.c_str()))
//		{
//		}
//		else {
//			filteredDEStatsDatasets.removeOne(dataset);
//		}
//	}
//
//
//	_deStatsDataset1Action.setDatasets(filteredDEStatsDatasets);
//	_deStatsDataset1Action.setPlaceHolderString("deStats dataset1");
//	_deStatsDataset2Action.setDatasets(filteredDEStatsDatasets);
//	_deStatsDataset2Action.setPlaceHolderString("deStats dataset2");
//	if (filteredDEStatsDatasets.isEmpty())
//	{
//		//_barSettingsAction.setEnabled(false);
//	}
//
//}

ParallelBarsOptionsAction::deStatsDataset1SelectionAction::Widget::Widget(QWidget* parent, deStatsDataset1SelectionAction* deStatsDataset1SelectAction) :
	WidgetActionWidget(parent, deStatsDataset1SelectAction)
{
	auto& ParallelBarsOptionsAction = deStatsDataset1SelectAction->_ParallelBarsOptionsAction;



	//selectiondeStats1Widget->setMaximumWidth(250);



	auto selectionExampledeStatsOptionLayout = new QFormLayout();
	selectionExampledeStatsOptionLayout->setContentsMargins(0, 0, 0, 0);


	selectionExampledeStatsOptionLayout->addRow(ParallelBarsOptionsAction._neighborhoodAction.createLabelWidget(this), ParallelBarsOptionsAction._neighborhoodAction.createWidget(this));

	selectionExampledeStatsOptionLayout->addRow(ParallelBarsOptionsAction._species1Name.createLabelWidget(this), ParallelBarsOptionsAction._species1Name.createWidget(this));

	selectionExampledeStatsOptionLayout->addRow(ParallelBarsOptionsAction._species2Name.createLabelWidget(this), ParallelBarsOptionsAction._species2Name.createWidget(this));

	selectionExampledeStatsOptionLayout->addRow(ParallelBarsOptionsAction._selectedCrossspeciescluster.createLabelWidget(this), ParallelBarsOptionsAction._selectedCrossspeciescluster.createWidget(this));

	selectionExampledeStatsOptionLayout->addRow(ParallelBarsOptionsAction._geneNameAction.createLabelWidget(this), ParallelBarsOptionsAction._geneNameAction.createWidget(this));

	auto selectiondeStats1Widget = ParallelBarsOptionsAction._deStatsDataset1Action.createWidget(this);
	selectiondeStats1Widget->findChild<QComboBox*>("ComboBox")->setSizeAdjustPolicy(QComboBox::AdjustToContents);
	auto selectiondeStats2Widget = ParallelBarsOptionsAction._deStatsDataset2Action.createWidget(this);
	selectiondeStats2Widget->findChild<QComboBox*>("ComboBox")->setSizeAdjustPolicy(QComboBox::AdjustToContents);
	selectionExampledeStatsOptionLayout->addRow(ParallelBarsOptionsAction._deStatsDataset1Action.createLabelWidget(this), selectiondeStats1Widget);

	selectionExampledeStatsOptionLayout->addRow(ParallelBarsOptionsAction._deStatsDataset2Action.createLabelWidget(this), selectiondeStats2Widget);

	setLayout(selectionExampledeStatsOptionLayout);
}

inline ParallelBarsOptionsAction::deStatsDataset1SelectionAction::deStatsDataset1SelectionAction(ParallelBarsOptionsAction& ParallelBarsOptionsAction) :
	WidgetAction(nullptr, "deStatsDataset1SelectionAction"),
	_ParallelBarsOptionsAction(ParallelBarsOptionsAction)
{
	setText("Options");
	setIcon(Application::getIconFont("FontAwesome").getIcon("database"));

}

void ParallelBarsOptionsAction::onDataEvent(hdps::DatasetEvent* dataEvent)
{
	if (dataEvent->getType() == hdps::EventType::DatasetAdded)
	{
		//updateDatasetPickerAction();
	}
	if (dataEvent->getType() == hdps::EventType::DatasetRemoved)
	{
		//updateDatasetPickerAction();
	}
	if (dataEvent->getType() == hdps::EventType::DatasetChildAdded)
	{
		//updateDatasetPickerAction();
	}
	if (dataEvent->getType() == hdps::EventType::DatasetChildRemoved)
	{
		//updateDatasetPickerAction();
	}
	if (dataEvent->getType() == hdps::EventType::DatasetDataChanged)
	{
		//updateDatasetPickerAction();
	}


}

void ParallelBarsOptionsAction::initLoader()
{

	updateData();
	_ParallelBarsViewerPlugin.getBarChartWidget().setSpecies1(_species1Name.getString());
	_ParallelBarsViewerPlugin.getBarChartWidget().setSpecies2(_species2Name.getString());

}

void ParallelBarsOptionsAction::fromVariantMap(const QVariantMap& variantMap)
{
	WidgetAction::fromVariantMap(variantMap);

	_deStatsDataset1Action.fromParentVariantMap(variantMap);
	_deStatsDataset2Action.fromParentVariantMap(variantMap);
	_geneNameAction.fromParentVariantMap(variantMap);
	_neighborhoodAction.fromParentVariantMap(variantMap);
	_selectedCrossspeciescluster.fromParentVariantMap(variantMap);
	_species1Name.fromParentVariantMap(variantMap);
	_species2Name.fromParentVariantMap(variantMap);
	_selectionColorAction.fromParentVariantMap(variantMap);
	initLoader();

}

QVariantMap ParallelBarsOptionsAction::toVariantMap() const
{
	QVariantMap variantMap = WidgetAction::toVariantMap();

	_deStatsDataset1Action.insertIntoVariantMap(variantMap);
	_deStatsDataset2Action.insertIntoVariantMap(variantMap);
	_neighborhoodAction.insertIntoVariantMap(variantMap);
	_geneNameAction.insertIntoVariantMap(variantMap);
	_selectedCrossspeciescluster.insertIntoVariantMap(variantMap);
	_species1Name.insertIntoVariantMap(variantMap);
	_species2Name.insertIntoVariantMap(variantMap);
	_selectionColorAction.insertIntoVariantMap(variantMap);
	return variantMap;
}