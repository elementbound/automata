attribute vec3 in_Position;
attribute vec2 in_TextureCoord;

varying vec2 v_Texcoord;

void main()
{
    gl_Position = gm_Matrices[MATRIX_WORLD_VIEW_PROJECTION] * vec4(in_Position.xyz, 1.0);
    
    v_Texcoord = in_TextureCoord;
}

//######################_==_YOYO_SHADER_MARKER_==_######################@~varying vec2 v_Texcoord;

uniform vec2 u_Range;

void main()
{
    vec4 dead_cell = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 live_cell = vec4(1.0, 1.0, 1.0, 1.0);
    
    float count = 
        texture2D(gm_BaseTexture, v_Texcoord + vec2( 1.0, 0.0) * u_Range).x + 
        texture2D(gm_BaseTexture, v_Texcoord + vec2( 1.0, 1.0) * u_Range).x + 
        texture2D(gm_BaseTexture, v_Texcoord + vec2( 0.0, 1.0) * u_Range).x + 
        texture2D(gm_BaseTexture, v_Texcoord + vec2(-1.0, 1.0) * u_Range).x + 
        texture2D(gm_BaseTexture, v_Texcoord + vec2(-1.0, 0.0) * u_Range).x + 
        texture2D(gm_BaseTexture, v_Texcoord + vec2(-1.0,-1.0) * u_Range).x + 
        texture2D(gm_BaseTexture, v_Texcoord + vec2( 0.0,-1.0) * u_Range).x + 
        texture2D(gm_BaseTexture, v_Texcoord + vec2( 1.0,-1.0) * u_Range).x;
        
    if(texture2D(gm_BaseTexture, v_Texcoord).x > 0.0) //live cell
    {
        if(count < 2.0)
            gl_FragColor = dead_cell;
        else if(count <= 3.0)
            gl_FragColor = live_cell;
        else
            gl_FragColor = dead_cell;
    }
    else //dead cell
    {
        if(count == 3.0)
            gl_FragColor = live_cell;
        else
            gl_FragColor = dead_cell;
    }
}

