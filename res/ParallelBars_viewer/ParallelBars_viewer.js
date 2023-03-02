var _data = null;
var flag = false;
var _dataQueue = new ParallelBarsViewerDataQueue(1, queueData);
var _defaultValues = null;
var yAxisrightSpace;
window.onresize = doALoadOfStuff;
var numOFBars;
var svg;
var tooltip;
var containerHeight
var yScaleTooltipSpecies;
var xScaleTooltipSpecies;
var species1Name = "";
var species2Name = "";
var species3Name = "";
var species4Name = "";
var species5Name = "";
var geneName = "";
var selectedCrossspeciescluster = "";
var ClusterStorage1 = {};
var ClusterStorage2 = {};
var ClusterStorage3 = {};
var ClusterStorage4 = {};
var ClusterStorage5 = {};


var topSpace = 1;
var rightSpace = 1;
var bottomSpace = 1;
var leftSpace = 1;
var middleTooltip = 20;
var axisStart = 65;
var species1BarStart = 75;
var buffer = 10;
var hTooltip;
var hTooltip;
var regionWidthTooltip;
var species2BarStart;
var species3BarStart;
var species4BarStart;
var species5BarStart;
const sortingCrossSpeciesClustersList = ["Lamp5_1", "Lamp5_2", "Lamp5_Lhx6_1", "Sncg_1", "Sncg_2", "Sncg_3", "Vip_1", "Vip_2", "Vip_3", "Vip_4", "Vip_5", "Vip_6", "Vip_7", "Vip_8", "Pax6_1", "Pax6_2","Sst Chodl_1", "Sst_1", "Sst_2", "Sst_3", "Sst_4", "Sst_5", "Sst_6", "Sst_7", "Sst_8", "Sst_9", "Pvalb_1", "Pvalb_2", "Pvalb_3", "Pvalb_4", "Chandelier_1","L2/3 IT_1", "L2/3 IT_2", "L2/3 IT_3", "L4 IT_1", "L4 IT_2", "L5 IT_1", "L5 IT_2", "L6 IT Car3_1", "L6 IT Car3_2", "L6 IT_1", "Astro_1", "Oligo_1", "VLMC_1", "Endo_1", "Micro-PVM_1", "OPC_1", "OPC_2", "L6 CT_1", "L6 CT_2", "L6b_1", "L6b_2", "L6b_3", "L5 ET_1", "L5 ET_2", "L5/6 NP_1", "L5/6 NP_2"];
//Qt Connections
try {
    new QWebChannel(qt.webChannelTransport, function (channel) {
        QtBridge = channel.objects.QtBridge;
        QtBridge.qt_setData.connect(function () { setData(arguments[0]); });
        QtBridge.qt_setSelectedCrossspeciescluster.connect(function () { setSelectedCrossspeciescluster(arguments[0]); });
        notifyBridgeAvailable();
    });
} catch (error) { isQtAvailable = false; }


function clickMiddleLabel(d) {

    if (selectedCrossspeciescluster == d) {
        selectedCrossspeciescluster = "";
        svg.select("#mouseclickSpecies1").remove();
        svg.select("#mouseclickSpecies2").remove();
        svg.select("#mouseclickSpecies3").remove();
        svg.select("#mouseclickSpecies4").remove();
        svg.select("#mouseclickSpecies5").remove();
        //svg
        //    .call(yAxisrightSpace)
        //    .selectAll("text")
        //    .style("fill", "black");
        if (isQtAvailable) {
            QtBridge.js_crossspeciesclusterSelection("");
        }
    }
    else {
        selectedCrossspeciescluster = d;
        selectBars(selectedCrossspeciescluster);
        if (isQtAvailable) {
            QtBridge.js_crossspeciesclusterSelection(selectedCrossspeciescluster);
        }
    }
}

function clickBar(d) {

    if (selectedCrossspeciescluster == d.clusterName) {
        selectedCrossspeciescluster = "";
        svg.select("#mouseclickSpecies1").remove();
        svg.select("#mouseclickSpecies2").remove();
        svg.select("#mouseclickSpecies3").remove();
        svg.select("#mouseclickSpecies4").remove();
        svg.select("#mouseclickSpecies5").remove();
        //svg
        //    .call(yAxisrightSpace)
        //    .selectAll("text")
        //    .style("fill", "black");
        if (isQtAvailable) {
            QtBridge.js_crossspeciesclusterSelection("");
        }
    }
    else {
        selectedCrossspeciescluster = d.clusterName;
        selectBars(selectedCrossspeciescluster);
        if (isQtAvailable) {
            QtBridge.js_crossspeciesclusterSelection(selectedCrossspeciescluster);
        }
    }
}

function selectBars(d) {
    svg.select("#mouseclickSpecies1").remove();
    svg.select("#mouseclickSpecies2").remove();
    svg.select("#mouseclickSpecies3").remove();
    svg.select("#mouseclickSpecies4").remove();
    svg.select("#mouseclickSpecies5").remove();
    //svg
    //    .call(yAxisrightSpace)
    //    .selectAll("text")
    //    .style("fill", "black");

    svg.append('rect')
        .attr("id", "mouseclickSpecies1")
        .attr('x', species1BarStart)
        .attr('y', yScaleTooltipSpecies(d))
        .attr('width', xScaleTooltipSpecies(ClusterStorage1[d]))
        .attr('height', yScaleTooltipSpecies.bandwidth())
        .attr('stroke', "#de2d26")
        .attr("stroke-width", 2)
        .attr('fill', 'none');


    svg.append('rect')
        .attr("id", "mouseclickSpecies2")
        .attr('x', species2BarStart)
        .attr('y', yScaleTooltipSpecies(d))
        .attr('width', xScaleTooltipSpecies(ClusterStorage2[d]))
        .attr('height', yScaleTooltipSpecies.bandwidth())
        .attr('stroke', "#de2d26")
        .attr("stroke-width", 2)
        .attr('fill', 'none');

    svg.append('rect')
        .attr("id", "mouseclickSpecies3")
        .attr('x', species3BarStart)
        .attr('y', yScaleTooltipSpecies(d))
        .attr('width', xScaleTooltipSpecies(ClusterStorage3[d]))
        .attr('height', yScaleTooltipSpecies.bandwidth())
        .attr('stroke', "#de2d26")
        .attr("stroke-width", 2)
        .attr('fill', 'none');

    svg.append('rect')
        .attr("id", "mouseclickSpecies4")
        .attr('x', species4BarStart)
        .attr('y', yScaleTooltipSpecies(d))
        .attr('width', xScaleTooltipSpecies(ClusterStorage4[d]))
        .attr('height', yScaleTooltipSpecies.bandwidth())
        .attr('stroke', "#de2d26")
        .attr("stroke-width", 2)
        .attr('fill', 'none');

    svg.append('rect')
        .attr("id", "mouseclickSpecies5")
        .attr('x', species5BarStart)
        .attr('y', yScaleTooltipSpecies(d))
        .attr('width', xScaleTooltipSpecies(ClusterStorage5[d]))
        .attr('height', yScaleTooltipSpecies.bandwidth())
        .attr('stroke', "#de2d26")
        .attr("stroke-width", 2)
        .attr('fill', 'none');

    //svg
    //    .call(yAxisrightSpace)
    //    .selectAll("text")
    //    .style("fill", function (m) {
    //        if (m == d) {
    //            return "#de2d26";
    //        }
    //        else {
    //            return "black";
    //        }
    //    });



    if (yScaleTooltipSpecies(d) > 1) {
        window.scrollTo(0, yScaleTooltipSpecies(d) - 1);
        }
        else {
        window.scrollTo(0, yScaleTooltipSpecies(d));
        }
}

function mouseleaveSpecies(d) {
    tooltip
        .style("opacity", 0)
}
function mousemoveSpecies(d) {
    tooltip.style("position", "absolute");

    if (d3.event.pageX > window.innerWidth / 2) {
        tooltip
            .style('left', (d3.event.pageX - 30) + 'px')
    }
    else {
        tooltip
            .style('left', (d3.event.pageX + 30) + 'px')
    }
    var temp;
    if (containerHeight / 2 > window.innerHeight / 2) {
        temp = containerHeight / 2;
    }
    else {
        temp = window.innerHeight / 2;
    }
    
    if (d3.event.pageY >= temp / 2) {
        tooltip
            .style('top', (d3.event.pageY - 30) + 'px')
    }
    else {
        tooltip
            .style('top', (d3.event.pageY + 30) + 'px')
    }

}

function mouseoverSpecies1(d) {
    tooltip
        .html("<div><b>" + d.species1ClusterCount+"</b></div>")
        .style("opacity", 1)
}

function mouseoverSpecies2(d) {
    tooltip
        .html("<div><b>" + d.species2ClusterCount + "</b></div>")
        .style("opacity", 1)
}

function mouseoverSpecies3(d) {
    tooltip
        .html("<div><b>" + d.species3ClusterCount + "</b></div>")
        .style("opacity", 1)
}

function mouseoverSpecies4(d) {
    tooltip
        .html("<div><b>" + d.species4ClusterCount + "</b></div>")
        .style("opacity", 1)
}

function mouseoverSpecies5(d) {
    tooltip
        .html("<div><b>" + d.species5ClusterCount + "</b></div>")
        .style("opacity", 1)
}

//Main Visualization
const ParallelBarsVis = () => {
    d3.select("g").remove();
    d3.select("svg").remove();
    svg = d3.select("#my_dataviz");
    svg.selectAll("*").remove();
    svg_Axes = d3.select("#my_dataviz_axes");
    svg_Axes.selectAll("*").remove();

    //var _data = [
    //    { clusterName: "L5 IT/3", species1ClusterCount: 10, species2ClusterCount: 20, species3ClusterCount: 30, species4ClusterCount: 40, species5ClusterCount: 50, clusterColor: "red", species1Name: "human", species2Name: "chimp", species3Name: "gorilla", species4Name: "rhesus", species5Name: "marmoset", geneName: "AAVF" },
    //    { clusterName: "2", species1ClusterCount: 70, species2ClusterCount: 40, species3ClusterCount: 26, species4ClusterCount: 47, species5ClusterCount: 520, clusterColor: "red", species1Name: "human", species2Name: "chimp", species3Name: "gorilla", species4Name: "rhesus", species5Name: "marmoset", geneName: "AAVF" },
    //    { clusterName: "3", species1ClusterCount: 90, species2ClusterCount: 24, species3ClusterCount: 36, species4ClusterCount: 90, species5ClusterCount: 204, clusterColor: "blue", species1Name: "human", species2Name: "chimp", species3Name: "gorilla", species4Name: "rhesus", species5Name: "marmoset", geneName: "AAVF" },
    //    { clusterName: "4", species1ClusterCount: 100, species2ClusterCount: 55, species3ClusterCount: 10, species4ClusterCount: 120, species5ClusterCount: 220, clusterColor: "green", species1Name: "human", species2Name: "chimp", species3Name: "gorilla", species4Name: "rhesus", species5Name: "marmoset", geneName: "AAVF" },
    //];
     numOFBars = _data.length;

    containerHeight = numOFBars * 20;

    wTooltip = (98 / 100) * window.innerWidth;
    hTooltip = containerHeight + topSpace + bottomSpace;
    regionWidthTooltip = (wTooltip - rightSpace - leftSpace - middleTooltip - species1BarStart - 50) / 5;
    species2BarStart = species1BarStart + regionWidthTooltip + buffer;
    species3BarStart = species2BarStart + regionWidthTooltip + buffer;
    species4BarStart = species3BarStart + regionWidthTooltip + buffer;
    species5BarStart = species4BarStart + regionWidthTooltip + buffer;




    svg = d3
        .select("#my_dataviz")
        .append("svg")
        .attr(
            "width",
            leftSpace + wTooltip + rightSpace
        )
        .attr(
            "height",
            topSpace + hTooltip + bottomSpace
        )
        .append("g")
        .attr(
            "transform",
            translation(leftSpace, topSpace)
        );
    tooltip = d3.select("#my_dataviz")
        .append("div")
        .style("opacity", 0)
        .attr("class", "tooltip")
        .style("background-color", "white")
        .style("border", "solid")
        .style("border-width", "1px")
        .style("border-radius", "5px");
    var maxValueTooltip = Math.max(
        d3.max(_data, function (d) {
            return d.species1ClusterCount;
        }),
        d3.max(_data, function (d) {
            return d.species2ClusterCount;
        }),
        d3.max(_data, function (d) {
            return d.species3ClusterCount;
        }),
        d3.max(_data, function (d) {
            return d.species4ClusterCount;
        }),
        d3.max(_data, function (d) {
            return d.species5ClusterCount;
        })
    );
    xScaleTooltipSpecies = d3
        .scaleLinear()
        .domain([0, maxValueTooltip])
        .range([0, regionWidthTooltip])
        .nice();

    yScaleTooltipSpecies = d3
        .scaleBand()
        .domain(
            _data.map(function (d) {
                return d.clusterName;
            })
        )
        .rangeRound([hTooltip, 0])
        .padding(0.1);
    var species1BarGroup = svg
        .append("g")
        .attr("transform", translation(species1BarStart, 0));
    var species2BarGroup = svg
        .append("g")
        .attr("transform", translation(species2BarStart, 0));
    var species3BarGroup = svg
        .append("g")
        .attr("transform", translation(species3BarStart, 0));
    var species4BarGroup = svg
        .append("g")
        .attr("transform", translation(species4BarStart, 0));
    var species5BarGroup = svg
        .append("g")
        .attr("transform", translation(species5BarStart, 0));
    var yAxisrightSpace = d3
        .axisLeft()
        .scale(yScaleTooltipSpecies)
        .tickSize(1, 0)
        .tickFormat("")
        .tickPadding(1);
    svg
        .append("g")
        .attr("class", "axis y right")
        .attr("transform", translation(axisStart, 0))  // start the axis from the extreme left
        .call(yAxisrightSpace)
        .selectAll("text")
        .attr("x", leftSpace - 10)  // move the labels to the right side of the axis
        .attr("dy", "0.32em")
        .style("cursor", "pointer")
        .on("click", clickMiddleLabel)
        .text(function (d) { return d; });  // add the clusterName text to the labels
    species1BarGroup
        .selectAll(".bar.right")
        .data(_data)
        .enter()
        .append("rect")
        .attr("class", "bar right")
        .attr("x", 0)
        .attr("y", function (d) {
            return yScaleTooltipSpecies(d.clusterName);
        })
        .attr("width", function (d) {
            return xScaleTooltipSpecies(d.species1ClusterCount);
        })
        .attr("fill", function (d) {
            return d.clusterColor;
        })
        .attr("height", yScaleTooltipSpecies.bandwidth())
        .style("cursor", "pointer")
        .on("click", clickBar)
        .on("mouseover", mouseoverSpecies1)
        .on("mousemove", mousemoveSpecies)
        .on("mouseleave", mouseleaveSpecies)
        ;

    species2BarGroup
        .selectAll(".bar.right")
        .data(_data)
        .enter()
        .append("rect")
        .attr("class", "bar right")
        .attr("x", 0)
        .attr("y", function (d) {
            return yScaleTooltipSpecies(d.clusterName);
        })
        .attr("width", function (d) {
            return xScaleTooltipSpecies(d.species2ClusterCount);
        })
        .attr("fill", function (d) {
            return d.clusterColor;
        })
        .attr("height", yScaleTooltipSpecies.bandwidth())
        .style("cursor", "pointer")
        .on("click", clickBar)
        .on("mouseover", mouseoverSpecies2)
        .on("mousemove", mousemoveSpecies)
        .on("mouseleave", mouseleaveSpecies)
        ;

    species3BarGroup
        .selectAll(".bar.right")
        .data(_data)
        .enter()
        .append("rect")
        .attr("class", "bar right")
        .attr("x", 0)
        .attr("y", function (d) {
            return yScaleTooltipSpecies(d.clusterName);
        })
        .attr("width", function (d) {
            return xScaleTooltipSpecies(d.species3ClusterCount);
        })
        .attr("fill", function (d) {
            return d.clusterColor;
        })
        .attr("height", yScaleTooltipSpecies.bandwidth())
        .style("cursor", "pointer")
        .on("click", clickBar)
        .on("mouseover", mouseoverSpecies3)
        .on("mousemove", mousemoveSpecies)
        .on("mouseleave", mouseleaveSpecies)
        ;

    species4BarGroup
        .selectAll(".bar.right")
        .data(_data)
        .enter()
        .append("rect")
        .attr("class", "bar right")
        .attr("x", 0)
        .attr("y", function (d) {
            return yScaleTooltipSpecies(d.clusterName);
        })
        .attr("width", function (d) {
            return xScaleTooltipSpecies(d.species4ClusterCount);
        })
        .attr("fill", function (d) {
            return d.clusterColor;
        })
        .attr("height", yScaleTooltipSpecies.bandwidth())
        .style("cursor", "pointer")
        .on("click", clickBar)
        .on("mouseover", mouseoverSpecies4)
        .on("mousemove", mousemoveSpecies)
        .on("mouseleave", mouseleaveSpecies)
        ;

    species5BarGroup
        .selectAll(".bar.right")
        .data(_data)
        .enter()
        .append("rect")
        .attr("class", "bar right")
        .attr("x", 0)
        .attr("y", function (d) {
            return yScaleTooltipSpecies(d.clusterName);
        })
        .attr("width", function (d) {
            return xScaleTooltipSpecies(d.species5ClusterCount);
        })
        .attr("fill", function (d) {
            return d.clusterColor;
        })
        .attr("height", yScaleTooltipSpecies.bandwidth())
        .style("cursor", "pointer")
        .on("click", clickBar)
        .on("mouseover", mouseoverSpecies5)
        .on("mousemove", mousemoveSpecies)
        .on("mouseleave", mouseleaveSpecies)
        ;


    function translation(x, y) {
        return "translate(" + x + "," + y + ")";
    }
    var svgAxis = d3
        .select("#my_dataviz_axes")
        .append("svg")
        .attr(
            "width",
            leftSpace + wTooltip + rightSpace
        )
        .attr("height", 50)
        .append("g")
        .attr(
            "transform",
            translation(leftSpace, topSpace)
        );
    var maxValueTooltip = Math.max(
        d3.max(_data, function (d) {
            return d.species1ClusterCount;
        }),
        d3.max(_data, function (d) {
            return d.species2ClusterCount;
        }),
        d3.max(_data, function (d) {
            return d.species3ClusterCount;
        }),
        d3.max(_data, function (d) {
            return d.species4ClusterCount;
        }),
        d3.max(_data, function (d) {
            return d.species5ClusterCount;
        })
    );
    var species1XAxisSpace = d3.axisTop().scale(xScaleTooltipSpecies).ticks(2);
    var species2XAxisSpace = d3.axisTop().scale(xScaleTooltipSpecies).ticks(2);
    var species3XAxisSpace = d3.axisTop().scale(xScaleTooltipSpecies).ticks(2);
    var species4XAxisSpace = d3.axisTop().scale(xScaleTooltipSpecies).ticks(2);
    var species5XAxisSpace = d3.axisTop().scale(xScaleTooltipSpecies).ticks(2);
    svgAxis
        .append("g")
        .attr("class", "axis x right")
        .attr("transform", translationAxes(species1BarStart, 20))
        .attr("shape-rendering", "crispEdges")
        .attr("fill", "black")
        .call(species1XAxisSpace)
        .selectAll("text")
        .attr("dy", ".21em");
    svgAxis
        .append("text")
        .attr("text-anchor", "middle")
        .attr("x", species1BarStart + regionWidthTooltip / 2)
        .attr("y", 30)
        .text(species1Name)
        .attr("font-size", "10");

    svgAxis
        .append("g")
        .attr("class", "axis x right")
        .attr("transform", translationAxes(species2BarStart, 20))
        .attr("shape-rendering", "crispEdges")
        .attr("fill", "black")
        .call(species2XAxisSpace)
        .selectAll("text")
        .attr("dy", ".21em");
    svgAxis
        .append("text")
        .attr("text-anchor", "middle")
        .attr("x", species2BarStart + regionWidthTooltip / 2)
        .attr("y", 30)
        .text(species2Name)
        .attr("font-size", "10");

    svgAxis
        .append("g")
        .attr("class", "axis x right")
        .attr("transform", translationAxes(species3BarStart, 20))
        .attr("shape-rendering", "crispEdges")
        .attr("fill", "black")
        .call(species3XAxisSpace)
        .selectAll("text")
        .attr("dy", ".21em");
    svgAxis
        .append("text")
        .attr("text-anchor", "middle")
        .attr("x", species3BarStart + regionWidthTooltip / 2)
        .attr("y", 30)
        .text(species3Name)
        .attr("font-size", "10");

    svgAxis
        .append("g")
        .attr("class", "axis x right")
        .attr("transform", translationAxes(species4BarStart, 20))
        .attr("shape-rendering", "crispEdges")
        .attr("fill", "black")
        .call(species4XAxisSpace)
        .selectAll("text")
        .attr("dy", ".21em");
    svgAxis
        .append("text")
        .attr("text-anchor", "middle")
        .attr("x", species4BarStart + regionWidthTooltip / 2)
        .attr("y", 30)
        .text(species4Name)
        .attr("font-size", "10");

    svgAxis
        .append("g")
        .attr("class", "axis x right")
        .attr("transform", translationAxes(species5BarStart, 20))
        .attr("shape-rendering", "crispEdges")
        .attr("fill", "black")
        .call(species5XAxisSpace)
        .selectAll("text")
        .attr("dy", ".21em");
    svgAxis
        .append("text")
        .attr("text-anchor", "middle")
        .attr("x", species5BarStart + regionWidthTooltip / 2)
        .attr("y", 30)
        .text(species5Name)
        .attr("font-size", "10");


    svgAxis
        .append("text")
        .attr("text-anchor", "middle")
        .attr("x", axisStart - 20)
        .attr("y", 32)
        .text("Gene")
        .attr("font-size", "10");
    svgAxis
        .append("text")
        .attr("text-anchor", "middle")
        .attr("x", axisStart - 20)
        .attr("y", 20)
        .text(geneName)
        .attr("font-size", "10");
    svgAxis.selectAll(".tick").each(function (d) {
        if (d === 0.0 || d === 0) {
            this.remove();
        }
    });
    function translationAxes(x, y) {
        return "translate(" + x + "," + y + ")";
    }
    if (selectedCrossspeciescluster !== "") {
        selectBars(selectedCrossspeciescluster);
    }

};

function setData(d) {
    _dataQueue.addData(d);
}

function setSelectedCrossspeciescluster(d) {
    if (d !== "") {
        selectedCrossspeciescluster = d;
        selectBars(selectedCrossspeciescluster);
    }
    else {
        selectedCrossspeciescluster = "";
        svg.select("#mouseclickSpecies1").remove();
        svg.select("#mouseclickSpecies2").remove();
        svg.select("#mouseclickSpecies3").remove();
        svg.select("#mouseclickSpecies4").remove();
        svg.select("#mouseclickSpecies5").remove();
        //svg
        //    .call(yAxisrightSpace)
        //    .selectAll("text")
        //    .style("fill", "black");
    }
}

function queueData(d) {
    _data = JSON.parse(d);
    for (var i = 0; i < _data.length; i++) {
        ClusterStorage1[_data[i].clusterName] = _data[i].species1ClusterCount;
        ClusterStorage2[_data[i].clusterName] = _data[i].species2ClusterCount;
        ClusterStorage3[_data[i].clusterName] = _data[i].species3ClusterCount;
        ClusterStorage4[_data[i].clusterName] = _data[i].species4ClusterCount;
        ClusterStorage5[_data[i].clusterName] = _data[i].species5ClusterCount;
    }

    _data.sort(function (a, b) {
        return sortingCrossSpeciesClustersList.indexOf(b.clusterName) - sortingCrossSpeciesClustersList.indexOf(a.clusterName)
    });

    species1Name = _data[0].species1Name;
    species2Name = _data[0].species2Name;
    species3Name = _data[0].species3Name;
    species4Name = _data[0].species4Name;
    species5Name = _data[0].species5Name;
    geneName = _data[0].geneName;
    ParallelBarsVis();
    flag = true;
}


//Resize on window dimension change
function doALoadOfStuff() {
    if (flag) { ParallelBarsVis(); }
}




