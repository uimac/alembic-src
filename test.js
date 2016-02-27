/*jslint devel:true nomen:true */
(function () {
	"use strict";
	var path = require("path"),
		abcio = require('alembic');

	function loadtest() {
		var file = "nurbs1.abc",
			path_list,
			i;
		console.log("hogehoge");
		console.log(abcio);
		abcio.load(file);
		path_list = abcio.get_nurbs_path_list(file);
		console.log(path_list);
		console.log(abcio.get_total_time(file));
		abcio.set_time(file, 1);
		/*
		for (i = 0; i < path_list.length; i = i + 1) {
			console.log(abcio.get_mesh(file, path_list[i]));
		}
		*/
	}
	loadtest();
}());
