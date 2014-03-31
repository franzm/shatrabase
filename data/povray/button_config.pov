#version 3.7; // for transparent background

//+W128 +H128 +A0.01 +AM2 +R5 +ua +D +P button_config.pov 

camera
{
	orthographic
	location z*-2
	look_at 0
	right x
	
	scale 2.1
}

light_source
{
	<1,1,-10>
	rgb 1
	
	area_light x*3, y*3, 4 4
}


#macro FINISH(bright_)

	pigment { rgb <0.1, 0.2, 0.1> }
	
	normal { bumps 0.15 scale 0.001 }
	
	finish 
	{ 
		ambient 0.0 diffuse 0.9 * bright_
		phong bright_
	}

#end


difference
{
	union
	{
		cylinder { 0,z, 1 }
		sphere { 0, 1 scale <1,1,0.1> }
	}
	
	// middle part
	cylinder { z*-0.1, z*0.5, 0.5 FINISH(0.5) }
	sphere { 0, 0.5 scale <1,1,0.1> translate z*0.5 FINISH(0.7) }
	 
	// cut-couts
	#local i=0;
	#while (i < 7)
	
		union 
		{
			cylinder { z*-0.2,z*1.01, 0.3 }
			sphere { 0,0.5 scale <1,1,0.2> }
			 
			translate x*1 rotate z * i*360/7 
		}
	
		#local i=i+1;
	#end	
	
	FINISH(1.0)
}

