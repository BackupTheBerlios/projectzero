/*
 * Project Zero - All Rights Reserved
 * render.h
 */

#ifndef DEF_RENDER_H
#define DEF_RENDER_H
class Render {
    public:
        Render::Render();
        Render::~Render();
        xmlBufferPtr doRender(class Line *);
    protected:
        void Render::doWrite(char * text);
        void Render::processLine(int x, int y, int width, int height, int prev);
        int numcel; 
        class Line * lines;
        xmlBufferPtr out;
};


#endif // DEF_RENDER_H

