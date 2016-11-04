#include <SFML/Graphics.hpp>
#include <SFML\OpenGL.hpp>

#define LEFT -1
#define RIGHT 1
#define NONE 0

int main(int argc, char *argv)
{
    sf::RenderWindow window(sf::VideoMode(400, 400), "QUADS", sf::Style::Titlebar | sf::Style::Close);

    /*sf::Texture dogeTexture;
    bool success = dogeTexture.loadFromFile("res/doge.jpg");

    sf::Sprite dogeSprite;
    dogeSprite.setTexture(dogeTexture);
    dogeSprite.setScale(0.25f, 0.25f);*/

    sf::Image dogeImage;
    dogeImage.loadFromFile("res/stevegibson.jpg");

    // Tell OpenGL that we want to use 2D textures. 
    glEnable(GL_TEXTURE_2D);

    // Tell OpenGL that we want to use lighting.
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);


    /* Tell OpenGL that we want to test the depth of each vertex to make sure that things that
    * are closer appear closer. */
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);

    glEnable(GL_CULL_FACE);

    glFrontFace(GL_CW);
    // Generate a texture in OpenGL, and retrieve its ID so that we can access it.
    GLuint dogeTextureID;
    glGenTextures(1, &dogeTextureID);

    /* 'Bind' our texture to the active texture space so that all of our operations
    * are done to it. */
    glBindTexture(GL_TEXTURE_2D, dogeTextureID);

    glTexImage2D(GL_TEXTURE_2D, // specifies that we're working on the active texture.
        0, // ignore.
        GL_RGBA, // specifies that making a texture that has RGBA data.
        dogeImage.getSize().x, // specify the width of the texture.
        dogeImage.getSize().y, // specify the height of the texture.
        0, // no border
        GL_RGBA, // specifies that we're working with an image that has RGBA data.
        GL_UNSIGNED_BYTE, // specifies the format of the RGBA data (in this case, everything is 0-255)
        dogeImage.getPixelsPtr() // specifies the actual image data.
        );

    /* Telling OpenGL that we want our active texture to magnify and minimize in
    * linear fashion. */
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    /* Telling OpenGL how we want to represent our texture when we try to draw past its bounds. */
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    float positionZ = 0.5f;
    float angleX = 0.0f;
    float angleY = 0.0f;
    int rotationX = NONE;
    int rotationY = NONE;
    int mode = GL_QUADS;

    float leftS = 0.00f;
    float rightS = 0.50f;
    float topT = 0.25f;
    float bottomT = 0.75f;

    sf::Event evt;
    sf::Clock appTimer;
    float deltaTime;
    while (window.isOpen()) {
        deltaTime = appTimer.restart().asSeconds();

        while (window.pollEvent(evt)) {
            if (evt.type == sf::Event::Closed) {
                window.close();
            }
            else if (evt.type == sf::Event::KeyPressed){
                if (evt.key.code == sf::Keyboard::Num1) {
                    mode = GL_TRIANGLES;
                    window.setTitle("TRIANGLES");
                }
                else if (evt.key.code == sf::Keyboard::Num2) {
                    mode = GL_TRIANGLE_STRIP;
                    window.setTitle("TRIANGLES STRIP");
                }
                else if (evt.key.code == sf::Keyboard::Num3) {
                    mode = GL_QUADS;
                    window.setTitle("QUADS");
                }
                else if (evt.key.code == sf::Keyboard::Num4) {
                    mode = GL_QUAD_STRIP;
                    window.setTitle("QUADS STRIP");
                }
                else if (evt.key.code == sf::Keyboard::Left && rotationY == NONE) rotationY = LEFT;
                else if (evt.key.code == sf::Keyboard::Right && rotationY == NONE) rotationY = RIGHT;
                else if (evt.key.code == sf::Keyboard::Up && rotationX == NONE) rotationX = LEFT;
                else if (evt.key.code == sf::Keyboard::Down && rotationX == NONE) rotationX = RIGHT;
            }
            else if (evt.type == sf::Event::KeyReleased){
                if (evt.key.code == sf::Keyboard::Left && rotationY == LEFT) rotationY = NONE;
                else if (evt.key.code == sf::Keyboard::Right && rotationY == RIGHT) rotationY = NONE;
                else if (evt.key.code == sf::Keyboard::Up && rotationX == LEFT) rotationX = NONE;
                else if (evt.key.code == sf::Keyboard::Down && rotationX == RIGHT) rotationX = NONE;
            }
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        GLenum error = glGetError();

        float offset = 0.25f;
        angleX += 90.0f * deltaTime * rotationX;
        angleY += 90.0f * deltaTime * rotationY;

        //draw stuff
        GLfloat position[] = { positionZ, -0.1, positionZ, 0.0f };
        glLightfv(GL_LIGHT0, GL_POSITION, position);

        glPushMatrix();
        glRotatef(angleX, 1.0f, 0.0f, 0.0f); // rotate at 90 degrees/second on the X axis depending on key held
        glRotatef(angleY, 0.0f, 1.0f, 0.0f); // rotate at 90 degrees/second on the Y axis depending on key held

        // Begin our drawing block.
        glBegin(mode);

        leftS += leftS > 0.5f ? 0.1f * deltaTime - 2.0f : 0.1f * deltaTime;
        rightS += rightS > 1.0f ? 0.1f * deltaTime - 2.0f : 0.1f * deltaTime;

        // Front
        glColor3f(1.0f, 0.0f, 0.0f);
        glTexCoord2f(leftS, bottomT);
        glNormal3f(0.0f, 0.0f, 1.0f);
        glVertex3f(-offset, -offset, offset);

        glTexCoord2f(leftS, topT);
        glNormal3f(0.0f, 0.0f, 1.0f);
        glVertex3f(-offset, offset, offset);

        glTexCoord2f(rightS, topT);
        glNormal3f(0.0f, 0.0f, 1.0f);
        glVertex3f(offset, offset, offset);

        glTexCoord2f(rightS, bottomT);
        glNormal3f(0.0f, 0.0f, 1.0f);
        glVertex3f(offset, -offset, offset);

        // Right
        glColor3f(0.0f, 1.0f, 0.0f);
        glTexCoord2f(leftS + 0.5f, bottomT);
        glNormal3f(1.0f, 0.0f, 0.0f);
        glVertex3f(offset, -offset, offset);

        glTexCoord2f(leftS + 0.5f, topT);
        glNormal3f(1.0f, 0.0f, 0.0f);
        glVertex3f(offset, offset, offset);

        glTexCoord2f(rightS + 0.5f, topT);
        glNormal3f(1.0f, 0.0f, 0.0f);
        glVertex3f(offset, offset, -offset);

        glTexCoord2f(rightS + 0.5f, bottomT);
        glNormal3f(1.0f, 0.0f, 0.0f);
        glVertex3f(offset, -offset, -offset);

        // Left
        glColor3f(0.0f, 0.0f, 1.0f);
        glTexCoord2f(leftS + 1.5f, bottomT);
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glVertex3f(-offset, -offset, -offset);

        glTexCoord2f(leftS + 1.5f, topT);
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glVertex3f(-offset, offset, -offset);

        glTexCoord2f(rightS + 1.5f, topT);
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glVertex3f(-offset, offset, offset);

        glTexCoord2f(rightS + 1.5f, bottomT);
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glVertex3f(-offset, -offset, offset);

        // Back
        glColor3f(0.0f, 1.0f, 1.0f);
        glTexCoord2f(leftS + 1.0f, bottomT);
        glNormal3f(0.0f, 0.0f, -1.0f);
        glVertex3f(offset, -offset, -offset);

        glTexCoord2f(leftS + 1.0f, topT);
        glNormal3f(0.0f, 0.0f, -1.0f);
        glVertex3f(offset, offset, -offset);

        glTexCoord2f(rightS + 1.0f, topT);
        glNormal3f(0.0f, 0.0f, -1.0f);
        glVertex3f(-offset, offset, -offset);

        glTexCoord2f(rightS + 1.0f, bottomT);
        glNormal3f(0.0f, 0.0f, -1.0f);
        glVertex3f(-offset, -offset, -offset);

        // Top
        glColor3f(1.0f, 0.0f, 1.0f);
        glTexCoord2f(leftS, bottomT);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f(-offset, offset, offset);

        glTexCoord2f(leftS, topT);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f(-offset, offset, -offset);

        glTexCoord2f(rightS, topT);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f(offset, offset, -offset);

        glTexCoord2f(rightS, bottomT);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f(offset, offset, offset);

        // Bottom
        glColor3f(1.0f, 1.0f, 1.0f);
        glTexCoord2f(leftS, bottomT);
        glNormal3f(0.0f, -1.0f, 0.0f);
        glVertex3f(-offset, -offset, -offset);

        glTexCoord2f(leftS, topT);
        glNormal3f(0.0f, -1.0f, 0.0f);
        glVertex3f(-offset, -offset, offset);

        glTexCoord2f(rightS, topT);
        glNormal3f(0.0f, -1.0f, 0.0f);
        glVertex3f(offset, -offset, offset);

        glTexCoord2f(rightS, bottomT);
        glNormal3f(0.0f, -1.0f, 0.0f);
        glVertex3f(offset, -offset, -offset);

        // End our drawing block.
        glEnd();

        glPopMatrix();

        window.display();
    }

    return 0;
}