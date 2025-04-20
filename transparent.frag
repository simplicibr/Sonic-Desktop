uniform sampler2D texture;
uniform vec4 colorkey; // Cor a ser descartada

void main() {
    vec4 pixel = texture2D(texture, gl_TexCoord[0].st);
    
    if (distance(pixel.rgb, colorkey.rgb) < 0.05) {
        discard;
    } else {
        gl_FragColor = pixel;
    }
}