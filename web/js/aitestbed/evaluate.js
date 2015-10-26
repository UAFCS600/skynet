function error_t(div,message)
{
	if(!div)
		return null;

	this.div=div;
	this.el=document.createElement("div");
	this.div.appendChild(this.el);

	this.glyph=document.createElement("span");
	this.el.appendChild(this.glyph);
	this.glyph.className="glyphicon glyphicon-remove"
	this.glyph.style.marginRight="5px";
	this.glyph.style.color="#a94442";

	this.label=document.createElement("label");
	this.el.appendChild(this.label);
	this.label.className="control-label has-feedback";
	this.label.innerHTML=message;
}

error_t.prototype.destroy=function()
{
	if(this.div)
		this.div.removeChild(this.el);

	this.div=this.el=null;
}

function evaluator_t(div)
{
	if(!div)
		return null;

	this.div=div;
	this.el=document.createElement("div");
	this.div.appendChild(this.el);

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
			input:document.createElement("input"),
			value:"1/(1+e^(-x))"
		},
		submit:
		{
			text:"Submit",
			button:document.createElement("button"),
			onclick:function(){myself.submit();}
		}
	};

	this.error_boxes=[];

	for(var key in this.data)
	{
		this.data[key].div=document.createElement("div");
		this.el.appendChild(this.data[key].div);
		this.data[key].div.className="form-group has-feedback";

		if(this.data[key].label)
		{
			this.data[key].div.appendChild(this.data[key].label);
			this.data[key].label.innerHTML=this.data[key].text;
		}

		if(this.data[key].input)
		{
			this.data[key].div.appendChild(this.data[key].input);
			this.data[key].input.className="form-control";
			this.data[key].input.value=this.data[key].value;
			this.data[key].input.onchange=function(){myself.validate();};
			this.data[key].input.onkeyup=function(){myself.validate();};
			this.data[key].input.onkeypress=function(){myself.validate();};

			if(this.data[key].width)
				this.data[key].input.style.width=this.data[key].width+"px";
			if(this.data[key].cols)
				this.data[key].input.cols=this.data[key].cols;
			if(this.data[key].rows)
				this.data[key].input.rows=this.data[key].rows;
		}

		if(this.data[key].button)
		{
			this.data[key].div.appendChild(this.data[key].button);
			this.data[key].button.className="btn btn-primary";
			this.data[key].button.innerHTML=this.data[key].text;
			this.data[key].button.onclick=this.data[key].onclick;

			if(this.data[key].width)
				this.data[key].button.style.width=this.data[key].width+"px";
		}
	}
}

evaluator_t.prototype.validate=function()
{
	this.data.submit.button.disabled=true;

	for(var key in this.data)
		this.data[key].div.className="form-group has-feedback";

	for(var key in this.error_boxes)
		if(this.error_boxes[key])
			this.error_boxes[key].destroy();

	this.error_boxes=[];

	var json=
	{
		layers:null,
		weights:null,
		inputs:null,
		sigmoid:null
	};

	try
	{
		var layer_length=null;

		try
		{
			layer_length=to_unsigned_int(this.data.layers.input.value);
		}
		catch(error)
		{
			this.data.layers.div.className="form-group has-feedback has-error";
			this.error_boxes.push(new error_t(this.data.layers.div,error));
			throw error;
		}

		try
		{
			json.layers=JSON.parse(this.data.topology.input.value);
		}
		catch(error)
		{
			this.data.topology.div.className="form-group has-feedback has-error";
			this.error_boxes.push(new error_t(this.data.topology.div,error));
			throw error;
		}

		try
		{
			json.weights=JSON.parse(this.data.weights.input.value);
		}
		catch(error)
		{
			this.data.weights.div.className="form-group has-feedback has-error";
			this.error_boxes.push(new error_t(this.data.weights.div,error));
			throw error;
		}

		try
		{
			json.inputs=JSON.parse(this.data.inputs.input.value);
		}
		catch(error)
		{
			this.data.inputs.div.className="form-group has-feedback has-error";
			this.error_boxes.push(new error_t(this.data.inputs.div,error));
			throw error;
		}

		json.sigmoid=this.data.sigmoid.input.value;

		if(json.layers.length!=layer_length)
		{
			var error="Number of layers and topology length do not match.";
			this.data.layers.div.className="form-group has-feedback has-error";
			this.data.topology.div.className="form-group has-feedback has-error";
			this.error_boxes.push(new error_t(this.data.layers.div,error));
			this.error_boxes.push(new error_t(this.data.topology.div,error));
			throw error;
		}

		if(json.layers.length<=0)
		{
			var error="Invalid number of layers(expected value >= 0).";
			this.data.layers.div.className="form-group has-feedback has-error";
			this.error_boxes.push(new error_t(this.data.layers.div,error));
			throw error;
		}

		if(json.layers[json.layers.length-1]!=1)
		{
			var error="Expected last layer value of 1(got "+json.layers[json.layers.length-1]+").";
			this.data.topology.div.className="form-group has-feedback has-error";
			this.error_boxes.push(new error_t(this.data.topology.div,error));
			throw error;
		}

		if(json.inputs.length!=json.layers[0])
		{
			var error="Invalid number of inputs(expected value "+json.layers[0]+" got "+json.inputs.length+")."
			this.data.inputs.div.className="form-group has-feedback has-error";
			this.error_boxes.push(new error_t(this.data.inputs.div,error));
			throw error;
		}

		var weight_length_correct=0;

		for(var ii=0;ii<json.layers.length;++ii)
			if(ii+1<json.layers.length)
				weight_length_correct+=json.layers[ii]*json.layers[ii+1];

		if(json.weights.length!=weight_length_correct)
		{
			var error="Invalid number of weights(expected "+weight_length_correct+" got "+json.weights.length+").";
			this.data.weights.div.className="form-group has-feedback has-error";
			this.error_boxes.push(new error_t(this.data.weights.div,error));
			throw error;
		}

		this.data.submit.button.disabled=false;
	}
	catch(error)
	{
	}

	return json;
}

evaluator_t.prototype.submit=function()
{
	var json=json=this.validate();
	var myself=this;

	request("?eval=true",json,
		function(json)
		{
			if(json.error)
			{
				var modal=new modal_ok_t(myself.el,"Server Error",json.error);
				modal.show();
			}
			else
			{
				var modal=new modal_ok_t(myself.el,"Evaluation");
				modal.get_content().appendChild(document.createTextNode("Output: "+json.output));
				modal.get_content().appendChild(document.createElement("br"));
				modal.get_content().appendChild(document.createTextNode("Time: "+json.ns+"ns"));
				modal.get_content().appendChild(document.createElement("br"));
				var boards_per_second=Math.ceil(1e9/json.ns);
				modal.get_content().appendChild(document.createTextNode("Board Evaluations Per Second: "+boards_per_second.toExponential()));
				modal.show();
			}
		},
		function(error)
		{
			var modal=new modal_ok_t(myself.el,"Send Error",error);
			modal.show();
		});
}
