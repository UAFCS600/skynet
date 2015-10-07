//$$y=x$$
//$$y=\frac{1}{1+e^{-x}}$$
//$$y=\frac{2}{1+e^{-x/2}}-1$$

function evaluator_t(div)
{
	if(!div)
		return null;

	this.div=div;
	this.el=document.createElement("div");

	var myself=this;

	this.data=
	{
		layers:
		{
			label:document.createElement("label"),
			text:"Layers",
			input:document.createElement("input"),
			value:"3"
		},
		topology:
		{
			label:document.createElement("label"),
			text:"Topology",
			input:document.createElement("input"),
			value:"[2,3,1]"
		},
		weights:
		{
			label:document.createElement("label"),
			text:"Weights",
			input:document.createElement("textarea"),
			value:"[1,1,1,1,1,1,1,1,1]",
			rows:10
		},
		inputs:
		{
			label:document.createElement("label"),
			text:"Inputs",
			input:document.createElement("textarea"),
			value:"[1,1]",
			rows:10
		},
		sigmoid:
		{
			label:document.createElement("label"),
			text:"Sigmoid",
			select:document.createElement("select"),
			options:
			[
				"y=x",
				"y=1/(1+e^(-x))",
				"y=2/(1+e^(-x/2))-1"
			]
		},
		submit:
		{
			text:"Submit",
			button:document.createElement("button"),
			onclick:function(){myself.submit();}
		}
	};

	this.div.appendChild(this.el);

	for(var key in this.data)
	{
		if(this.data[key].label)
		{
			this.el.appendChild(this.data[key].label);
			this.data[key].label.innerHTML=this.data[key].text;
			this.el.appendChild(document.createElement("br"));
		}

		if(this.data[key].input)
		{
			this.el.appendChild(this.data[key].input);
			this.data[key].input.className="form-control";
			this.data[key].input.value=this.data[key].value;
			this.el.appendChild(document.createElement("br"));

			if(this.data[key].width)
				this.data[key].input.style.width=this.data[key].width+"px";
			if(this.data[key].cols)
				this.data[key].input.cols=this.data[key].cols;
			if(this.data[key].rows)
				this.data[key].input.rows=this.data[key].rows;
		}

		if(this.data[key].select)
		{
			this.el.appendChild(this.data[key].select);
			this.data[key].select.className="form-control";

			for(var key2 in this.data[key].options)
			{
				var option=document.createElement("option");
				option.innerHTML=this.data[key].options[key2];
				this.data[key].select.appendChild(option);
			}
			this.el.appendChild(document.createElement("br"));

			if(this.data[key].width)
				this.data[key].select.style.width=this.data[key].width+"px";
		}

		if(this.data[key].button)
		{
			this.el.appendChild(this.data[key].button);
			this.data[key].button.className="btn btn-primary";
			this.data[key].button.innerHTML=this.data[key].text;
			this.data[key].button.onclick=this.data[key].onclick;
			this.el.appendChild(document.createElement("br"));

			if(this.data[key].width)
				this.data[key].button.style.width=this.data[key].width+"px";
		}
	}
}

evaluator_t.prototype.submit=function()
{
	var layer_length=null;
	var json=
	{
		layers:null,
		weights:null,
		inputs:null,
		sigmoid_index:null
	};

	try
	{
		layer_length=to_unsigned_int(this.data.layers.input.value);
	}
	catch(error)
	{
		throw "Invalid number of layers ("+error+").";
	}

	try
	{
		json.layers=JSON.parse(this.data.topology.input.value);
	}
	catch(error)
	{
		throw "Invalid layers: "+error;
	}

	try
	{
		json.weights=JSON.parse(this.data.weights.input.value);
	}
	catch(error)
	{
		throw "Invalid weights: "+error;
	}

	try
	{
		json.inputs=JSON.parse(this.data.inputs.input.value);
	}
	catch(error)
	{
		throw "Invalid inputs: "+error;
	}

	json.sigmoid_index=this.data.sigmoid.select.selectedIndex;

	if(json.layers.length!=layer_length)
		throw "Layer lengths do not match.";

	if(json.layers.length<=0)
		throw "Invalid layer length(expected value >= 0).";

	if(json.layers[json.layers.length-1]!=1)
		throw "Expected last layer value of 1(got "+json.layers[json.layers.length-1]+").";

	if(json.inputs.length!=json.layers[0])
		throw "Invalid input length(expected value "+json.layers[0]+
			" got "+json.inputs.length+").";

	var weight_length_correct=0;

	for(var ii=0;ii<json.layers.length;++ii)
		if(ii+1<json.layers.length)
			weight_length_correct+=json.layers[ii]*json.layers[ii+1];

	if(json.weights.length!=weight_length_correct)
		throw "Weight lengths do not match(expected "+weight_length_correct+
			" got "+json.weights.length+").";

	request("?eval=true",json,
		function(json)
		{
			console.log("JSON - "+JSON.stringify(json));
			alert("Output: "+json.output+"\nTime: "+json.ns+"ns");
		},
		function(error)
		{
			console.log("Error - "+error);
		});
}
