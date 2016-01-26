//
//  Shader.fsh
//  GameJamTemplateApp
//
//  Created by localadmin on 24/01/2016.
//  Copyright © 2016 localadmin. All rights reserved.
//

varying lowp vec4 colorVarying;

void main()
{
    gl_FragColor = colorVarying;
}
