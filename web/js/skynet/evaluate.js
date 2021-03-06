//$$y=x$$
//$$y=\frac{a}{1+e^{-x/b}}+c$$

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
			radiogroup:
			[
				{
					innerHTML:"<img src='/images/0.png'/>",
					fields:{}
				},
				{
					innerHTML:"<img src='/images/1.png'/>",
					fields:
					{
						a:{input:null,value:1,zero_ok:true},
						b:{input:null,value:1,zero_ok:false},
						c:{input:null,value:0,zero_ok:true}
					}
				}
			],
			radios:[]
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

			this.data[key].input.style.fontFamily="monospace";
			this.data[key].input.spellcheck=false;
		}

		var first_checked=false;

		if(this.data[key].radiogroup)
		{
			for(var key2 in this.data[key].radiogroup)
			{
				var radio_div=document.createElement("div");
				radio_div.style.marginTop="-40px";
				this.data[key].div.appendChild(radio_div);

				var radio=document.createElement("input");
				radio_div.appendChild(radio);
				this.data[key].radios.push(radio);
				radio.type="radio";
				radio.name="sigmoid";
				radio.className="radio";
				radio.style.display="inline";
				radio.style.verticalAlign="middle";
				radio.style.height="100px";
				radio.style.marginRight="10px";

				if(!first_checked)
				{
					first_checked=true;
					radio.checked=true;
				}

				var option=this.data[key].radiogroup[key2];

				var label=document.createElement("div");
				label.style.display="inline";
				label.innerHTML=option.innerHTML;
				radio_div.appendChild(label);

				var field_div=document.createElement("div");
				field_div.style.float="right";
				field_div.style.width="320px";
				field_div.style.marginTop="30px";
				label.appendChild(field_div);

				var input_group=document.createElement("div");
				input_group.className="input-group";
				field_div.appendChild(input_group);

				var first=true;

				for(key3 in option.fields)
				{
					var field=option.fields[key3];

					var field_label=document.createElement("span");
					field_label.className="input-group-addon nohighlight";
					field_label.innerHTML=key3;
					field_label.style.cursor="default";
					field_label.style.height="32px";
					if(!first)
						field_label.style.borderLeft="0px";
					field_label.style.borderRight="0px";
					input_group.appendChild(field_label);

					field.input=document.createElement("input");
					field.input.className="form-control";
					field.input.type="text";
					field.input.value=field.value;
					field.input.disabled=true;
					field.input.style.height="32px";
					field.input.style.paddingRight="5px";
					field.input.onchange=function(){myself.validate()};
					field.input.onkeyup=function(){myself.validate()};
					input_group.appendChild(field.input);

					if(first)
						first=false;
				}
			}
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

	//Hard-coded hack to make the input fields disable...
	this.data.sigmoid.radios[0].onchange=function()
	{
		if(this.checked)
		{
			myself.data.sigmoid.radiogroup[1].fields.a.input.disabled=true;
			myself.data.sigmoid.radiogroup[1].fields.b.input.disabled=true;
			myself.data.sigmoid.radiogroup[1].fields.c.input.disabled=true;
			myself.validate();
		}
	};
	this.data.sigmoid.radios[1].onchange=function()
	{
		if(this.checked)
		{
			myself.data.sigmoid.radiogroup[1].fields.a.input.disabled=false;
			myself.data.sigmoid.radiogroup[1].fields.b.input.disabled=false;
			myself.data.sigmoid.radiogroup[1].fields.c.input.disabled=false;
			myself.validate();
		}
	};
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
		sigmoid_index:null,
		a:1,
		b:1,
		c:0
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

		json.sigmoid_index=null;
		var sigmoid_counter=0;

		for(var key in this.data.sigmoid.radios)
		{
			if(this.data.sigmoid.radios[key].checked)
			{
				json.sigmoid_index=sigmoid_counter;
				break;
			}

			++sigmoid_counter;
		}

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

		try
		{
			if(this.data.sigmoid.radios[1].checked)
			{
				var fields=this.data.sigmoid.radiogroup[1].fields;

				for(var key in fields)
				{
					if(!is_float(fields[key].input.value)&&!is_int(fields[key].input.value))
						throw "Constant \""+key+"\" is not a float.";
					if(!fields[key].zero_ok&&parseFloat(fields[key].input.value)==0)
						throw "Division by zero for constant \""+key+"\".";
				}

				json.a=parseFloat(fields.a.input.value);
				json.b=parseFloat(fields.b.input.value);
				json.c=parseFloat(fields.c.input.value);
			}
		}
		catch(error)
		{
			this.data.sigmoid.div.className="form-group has-feedback has-error";
			this.error_boxes.push(new error_t(this.data.sigmoid.div,error));
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
	var json=this.validate();
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
