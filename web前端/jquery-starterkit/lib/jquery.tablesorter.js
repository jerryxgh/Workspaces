/*
 *
 * TableSorter - Client-side table sorting with ease!
 *
 * Copyright (c) 2006 Christian Bach (http://motherrussia.polyester.se)
 * Licensed under the MIT License:
 * http://www.opensource.org/licenses/mit-license.php
 * 
 * $Date: 2006-08-21 14:43:23 +0000 (m氓, 21 aug 2006) $
 * $Author: Christian $
 * 
 */
$.fn.tableSorter = function(o) {
	
	var defaults =  {
		sortDir: 0,
		sortColumn: null,
		sortClassAsc: 'ascending',
		sortClassDesc: 'descending',
		headerClass: null,
		stripingRowClass: false,
		highlightClass: false,
		rowLimit: 0,
		minRowsForWaitingMsg: 0,
		disableHeader: false,
		stripRowsOnStartUp: false,
		columnParser: false,
		dateFormat: 'mm/dd/yyyy'
	};		
	
	return this.each(function(){
		var COLUMN_DATA = [];
		var COLUMN_CACHE = [];
		var COLUMN_INDEX;
		var COLUMN_SORTER_CACHE = [];
		var COLUMN_CELL;
		var COLUMN_DIR;
		var COLUMN_HEADER_LENGTH;
		var COLUMN_LAST_INDEX = -1;
		jQuery.extend(defaults, o);
		var oTable = this;				
		
		if(defaults.stripRowsOnStartUp && defaults.stripingRowClass) {
			$.tableSorter.utils.stripRows(oTable,defaults);
		}
		var tableRowLength = oTable.rows.length-1;
			
		buildColumnDataIndex();
		
		function buildColumnHeaders() {
			var oFirstTableRow = oTable.rows[0];
			var oDataSampleRow = oTable.rows[1];
			COLUMN_HEADER_LENGTH = oFirstTableRow.cells.length;
			for( var i=0; i < COLUMN_HEADER_LENGTH; i++ ) {	
				var oCell = oFirstTableRow.cells[i];
				if(!$.tableSorter.utils.isHeaderDisabled(oCell,defaults.disableHeader,i)) {
					var oCellValue = $.tableSorter.utils.getElementText(oDataSampleRow.cells[i]);
					if(typeof(defaults.sortColumn) == "string") {
						if(defaults.sortColumn.toLowerCase() == $.tableSorter.utils.getElementText(oCell).toLowerCase()) {
							defaults.sortColumn = i;
						}
					}
					COLUMN_SORTER_CACHE[i] = $.tableSorter.analyzer.analyseString(oCellValue,defaults);
					if(defaults.columnParser) {
						var a = defaults.columnParser;
						var l = a.length;
						for(var j=0; j < l; j++) {
							if(i == a[j][0]) {
								COLUMN_SORTER_CACHE[i] = $.tableSorter.analyzer.getById(a[j][1]);
								continue;
							}  
						} 	
					}
					
					if(defaults.headerClass) {
						$(oCell).addClass(defaults.headerClass);
					}
					oCell.index = i;
					oCell.count = defaults.sortDir;
					$(oCell).click(function() {
						sortOnColumn( this, (this.count++) % 2, this.index );
					}); 
				}
			}
			addColGroup(oFirstTableRow);
			if(defaults.sortColumn != null) {
				$(oFirstTableRow.cells[defaults.sortColumn]).trigger("click");	
			}
			
		}
		function buildColumnDataIndex() {
			var l = oTable.rows.length;
			for (var i=1;i < l; i++) {
				COLUMN_DATA.push(oTable.rows[i]); 
			}
			buildColumnHeaders();
		}
		function addColGroup(columnsHeader) {
			var oSampleTableRow = oTable.rows[1];
			for(var i=0; i < COLUMN_HEADER_LENGTH; i++) {
				$(columnsHeader.cells[i]).css("width",oSampleTableRow.cells[i].clientWidth + "px");
			}
		}
		function sortOnColumn(oCell,dir,index) {
			if(tableRowLength > defaults.minRowsForWaitingMsg) {
				$.event.trigger( "sortStart");
			}
			COLUMN_INDEX = index;
			COLUMN_CELL = oCell;
			COLUMN_DIR = dir;
			$("th."+defaults.sortClassAsc,oTable).removeClass(defaults.sortClassAsc);
			$("th."+defaults.sortClassDesc,oTable).removeClass(defaults.sortClassDesc);
			$(COLUMN_CELL).addClass((dir % 2 ? defaults.sortClassAsc : defaults.sortClassDesc));
			if(defaults.highlightClass) {
				if(COLUMN_LAST_INDEX != COLUMN_INDEX && COLUMN_LAST_INDEX > -1) {
					$("tbody/tr",o).find("td:eq(" + COLUMN_LAST_INDEX + ")").removeClass(defaults.highlightClass).end();
				}
			}
			setTimeout(doSorting,0);
		}
		function doSorting() {
			var columns;
			if($.tableSorter.cache.exist(COLUMN_CACHE,COLUMN_INDEX)) {
				var cache = $.tableSorter.cache.get(COLUMN_CACHE,COLUMN_INDEX);
				if(cache.dir == COLUMN_DIR) {
					columns = cache.data;
					cache.dir = COLUMN_DIR;
				} else {
					columns = cache.data.reverse();
					cache.dir = COLUMN_DIR;
				}
			} else {
				var flatData = $.tableSorter.data.flatten(COLUMN_DATA,COLUMN_SORTER_CACHE,COLUMN_INDEX);
				flatData.sort(COLUMN_SORTER_CACHE[COLUMN_INDEX].sorter);
				if(defaults.sortDir) { flatData.reverse();}
				columns = $.tableSorter.data.rebuild(COLUMN_DATA,flatData,COLUMN_INDEX,COLUMN_LAST_INDEX);
				$.tableSorter.cache.add(COLUMN_CACHE,COLUMN_INDEX,COLUMN_DIR,columns);
				flatData = null;
			}
			$.tableSorter.utils.appendToTable(oTable,columns,defaults,COLUMN_INDEX,COLUMN_LAST_INDEX);

			columns = null;
			if(tableRowLength > defaults.minRowsForWaitingMsg) {
				$.event.trigger("sortStop",[COLUMN_INDEX]);
			}
			COLUMN_LAST_INDEX = COLUMN_INDEX;
			
		}
	});
};

$.fn.sortStart = function(fn) {
	return this.bind("sortStart",fn);
};
$.fn.sortStop = function(fn) {
	return this.bind("sortStop",fn);
};

$.tableSorter = {
	params: {},
	cache: {
		add: function(cache,index,dir,data) {
			var oCache = {};
			oCache.dir = dir;
			oCache.data = data;
			cache[index] = oCache;
		},
		get: function (cache,index) {
			return cache[index]; 
		},
		exist: function(cache,index) {
			var oCache = cache[index];
			if(!oCache) {
				return false
			} else {
				return true
			}
		}
	},
	data: {

		flatten: function(columnData,columnCache,columnIndex) {
			var flatData = [];
			var l = columnData.length;
			for (var i=0;i < l; i++) {
				flatData.push([i,columnCache[columnIndex].format($.tableSorter.utils.getElementText(columnData[i].cells[columnIndex]))]);
			}
			return flatData;
		},
		rebuild: function(columnData,flatData,columnIndex,columnLastIndex) {
			var l = flatData.length;
			var sortedData = [];
			for (var i=0;i < l; i++) {
				sortedData.push(columnData[flatData[i][0]]);
			}
			return sortedData;
		}
	},

	sorters: {},
	
	parsers: {},
	
	analyzer: {
	
		analyzers: [],
		add: function(analyzer) {
			this.analyzers.push(analyzer);
		},
		analyseString: function(s,params) {
			$.tableSorter.utils.setParams(params);
			var l=this.analyzers.length;
			var foundAnalyzer = false;
			for(var i=0; i < l; i++) {
				
				var analyzer = this.analyzers[i];	
				
				if(analyzer.is(s)) {
					foundAnalyzer = true;
					return analyzer;
					continue;
				}
			}
			if(!foundAnalyzer) {
				return $.tableSorter.parsers.generic;
			}
		},
		getById: function(s) {
			var l=this.analyzers.length;
			for(var i=0; i < l; i++) {
				var analyzer = this.analyzers[i];	
				if(analyzer.id == s) {
					return analyzer;
					continue;
				}	
			}
		}	
	},
	utils: {
		setParams: function(o) {
			 $.tableSorter.params = o;
		},
		getParams: function() {
			return $.tableSorter.params;
		},
		getElementText: function(o) {
			return o.innerHTML;
		},
		appendToTable: function(o,c,defaults,index,lastIndex) {
			var l = c.length;
			$("tbody",o).append(c);
			if(defaults.stripingRowClass) {
				$("tbody/tr",o).removeClass(defaults.stripingRowClass[0]).removeClass(defaults.stripingRowClass[1]);
				$.tableSorter.utils.stripRows(o,defaults);
			}
			if(defaults.highlightClass) {
				$("tbody/tr",o).find("td:eq(" + index + ")").addClass(defaults.highlightClass).end();
			}
			c=null;
		},
		stripRows: function(o,defaults) {
			$("tbody/tr:even",o).addClass(defaults.stripingRowClass[0]);
			$("tbody/tr:odd",o).addClass(defaults.stripingRowClass[1]);
		},
		isHeaderDisabled: function(o,arg,index) {
			if(typeof(arg) == "number") {
				return (arg == index)? true : false;
			} else if(typeof(arg) == "string") {
				return (arg.toLowerCase() == $.tableSorter.utils.getElementText(o).toLowerCase()) ? true : false;
			} else if(typeof(arg) == "object") {
				var l = arg.length;
				if(!this.lastFound) { this.lastFound = -1; }
				for(var i=0; i < l; i++) {
					var val = $.tableSorter.utils.isHeaderDisabled(o,arg[i],index);
					if(this.lastFound != i && val) {
						this.lastFound = i;
						return val;
					}
				}
			} else {
				return false	
			}
		}
	}
};
$.tableSorter.sorters.generic = function(a,b) {
	return ((a[1] < b[1]) ? -1 : ((a[1] > b[1]) ? 1 : 0));
};
$.tableSorter.sorters.numeric = function(a,b) { 
	return a[1]-b[1];
};
$.tableSorter.parsers.generic = {
	id: 'generic',
	is: function(s) {
		return true;
	},
	format: function(s) {
		return s.toLowerCase();
	},
	sorter: $.tableSorter.sorters.generic
};
$.tableSorter.parsers.currency = {
	id: 'currency',
	is: function(s) {
		return s.match(/^[?]/);
	},
	format: function(s) {
		return parseFloat(s.replace(/[^0-9.]/g,''));
	},
	sorter: $.tableSorter.sorters.numeric
};
$.tableSorter.parsers.numeric = {
	id: 'numeric',
	is: function(s) {
		return s.match(/^\b\d+\b$/);
	},
	format: function(s) {
		return parseFloat(s);
	},
	sorter: $.tableSorter.sorters.numeric
};
$.tableSorter.parsers.ipAddress = {
	id: 'ipAddress',
	is: function(s) {
		return s.match(/^\d{2,3}[\.]\d{2,3}[\.]\d{2,3}[\.]\d{2,3}$/);
	},
	format: function(s) {
		var a = s.split('.');
		var r = '';
		for (var i = 0, item; item = a[i]; i++) {
		   if(item.length == 2) {
				r += '0' + item;
		   } else {
				r += item;
		   }
		}	
		return parseFloat(r);
	},
	sorter: $.tableSorter.sorters.numeric
};
$.tableSorter.parsers.url = {
	id: 'url',
	is: function(s) {
		return s.match(/(https?|ftp|file):\/\//);
	},
	format: function(s) {
		return s.replace(/(https?|ftp|file):\/\//,'');
	},
	sorter: $.tableSorter.sorters.generic
};
$.tableSorter.parsers.isoDate = {
	id: 'isoDate',
	is: function(s) {
		return s.match(/^\d{4}[/-]\d{1,2}[/-]\d{1,2}$/)
	},
	format: function(s) {
		return parseFloat(new Date(s.replace(/-/g,'/')).getTime());
	},
	sorter: $.tableSorter.sorters.numeric
};
$.tableSorter.parsers.usLongDate = {
	id: 'usLongDate',
	is: function(s) {
		return s.match(/^[A-Za-z]{3,10}\.? [0-9]{1,2}, ([0-9]{4}|'?[0-9]{2}) (([0-2]?[0-9]:[0-5][0-9])|([0-1]?[0-9]:[0-5][0-9]\s(AM|PM)))$/);
	},
	format: function(s) {
		return parseFloat((new Date(s)).getTime());
	},
	sorter: $.tableSorter.sorters.numeric
}; 
$.tableSorter.parsers.shortDate = {
	id: 'shortDate',
	is: function(s) {
		return s.match(/^\d{1,2}[/-]\d{1,2}[/-]\d{4}$/);
	},
	format: function(s) {
		s = s.replace(/-/g,'/');
		var defaults = $.tableSorter.utils.getParams();
		if(defaults.dateFormat == "mm/dd/yyyy" || defaults.dateFormat == "mm-dd-yyyy") {
			s = s.replace(/(\d{1,2})[/-](\d{1,2})[/-](\d{4})/, '$3/$1/$2');
		} else if(defaults.dateFormat == "dd/mm/yyyy" || defaults.dateFormat == "dd-mm-yyyy") {
			s = s.replace(/(\d{1,2})[/-](\d{1,2})[/-](\d{4})/, '$3/$2/$1');
		}
		return parseFloat((new Date(s)).getTime());
	},
	sorter: $.tableSorter.sorters.numeric
};
$.tableSorter.parsers.time = {
    id: 'time',
    is: function(s) {
        return s.toUpperCase().match(/^(([0-2]?[0-9]:[0-5][0-9])|([0-1]?[0-9]:[0-5][0-9]\s(AM|PM)))$/);
    },
    format: function(s) {
        return parseFloat((new Date("2000/01/01 " + s)).getTime());
    },
    sorter: $.tableSorter.sorters.numeric
};
$.tableSorter.analyzer.add($.tableSorter.parsers.currency);
$.tableSorter.analyzer.add($.tableSorter.parsers.numeric);
$.tableSorter.analyzer.add($.tableSorter.parsers.isoDate);
$.tableSorter.analyzer.add($.tableSorter.parsers.shortDate);
$.tableSorter.analyzer.add($.tableSorter.parsers.usLongDate);
$.tableSorter.analyzer.add($.tableSorter.parsers.ipAddress);
$.tableSorter.analyzer.add($.tableSorter.parsers.url);
$.tableSorter.analyzer.add($.tableSorter.parsers.time);