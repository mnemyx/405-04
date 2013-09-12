COLOR_C shader(Collision coll, Matter matt, Light lit) {
	Vector3d theLight;				
	COLOR_C id, is, lightxmat, finalIntensity;
	Vector3d specLight, viewVec;
	double diff, spec;
	double ks = 1.0;
	double kd = 1.0;
	double ka = 1.0;
	
	// fix the ray info accordingly 
	if(lit.type == 0) {
		theLight = lit.info.normalize();
	} else if (lit.type == 1) {
		theLight = (coll.hitpoint - lit.info).normalize();
	} else {
		theLight = -lit.info;
	}
	
	// ambient lighting
	lightxmat.set(matt.color * matt.ca);
	finalIntensity.set(ka * lightxmat);
	
	// do diffuse lighting
	lightxmat.set((matt.color * matt.cd) * lit.color);
	
	if ((diff = theLight * coll.normal) < 0.0) {
		id.set(-diff * lightxmat);
		finalIntensity.set(finalIntensity + (kd * id));
	}
	
	// do the specular lighting
	lightxmat.set(matt.cs * lit.color);
	specLight = theLight - 2 * (theLight * coll.normal) * coll.normal;
	viewVec = (camera.vPoint - coll.hitpoint).normalize();
	
	if ((spec = specLight * viewVec) > 0.0) {
		is.set((pow(spec, matt.n)) * lightxmat);
		finalIntensity.set(finalIntensity + (ks * is));
	}
	
	return finalIntensity;
}


COLOR_C recShader(int obj, Collision collided, Material matter, Vector3d ur, int level) {
	int k;
	int objSp = -1;
	COLOR_C colorTemp, reflectColor;
	RAY p;
	Vector3d um;
	Vector blank(0.0,0.0,0.0);
	Collision tempC(INFINITY, blank, blank);
	Collision secCollide(INFINITY, blank, blank);
	Material tempM(&black, &black, &black, &black, 0.0);
	Light tempL;
	
	if(level > MAXLEVEL) 
		return (black);
		
	um = ur - 2 * (ur * collided.normal) * collided.normal;
	p.set(collided.hit, um);
	
	for(k = 0; k < numSp; k++) {
		if((sphere[k].spCheck(p, &tempC)) == 1) {
			if(tempC.t < secCollide.t) {
				secCollide.set(tempC);
				tempM.set(sphere[k].material);
				objSp = k;
			}
		}
	}
	
	if(secCollide.t < INFINITY) {
		reflectColor.set(recursive_shader(objSp, secCollide, tempM, um, level+1);
		tempL.set(4, &reflectColor, &um);
		colorTemp.set(shader(collided, matter, tempL);
	} else {
		colorTemp.set(black);
	}
	
	
	for(k = 0 ; k < numLi; k++) {
		if(objSp == -1) {
			colorTemp.set(colorTemp + shader(collided, matter, lights[k]));
		}
	}
	
	return colorTemp;
}
