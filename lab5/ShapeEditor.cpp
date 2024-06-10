#include "ShapeEditor.h"

#include <vector>

enum CurrentPaintingFigure
{
    NONE,
    RECTANGLE,
    ELLIPSE,
    LINE,
    DOT
};

std::uint32_t countObjects = 0; 

Shape* pchape[129]; 

CurrentPaintingFigure current = CurrentPaintingFigure::NONE;

ShapeObjectsEditor::ShapeObjectsEditor() 
{
    shape_ = nullptr; 
    shapeeditor_ = nullptr; 
}; 
ShapeObjectsEditor::~ShapeObjectsEditor() 
{
    delete shape_;
    delete shapeeditor_;
};

void ShapeObjectsEditor::StartLineEditor(HWND hwnd, UINT checkItem)
{
    current = CurrentPaintingFigure::LINE;
    delete shape_;
    shape_ = new LineShape;
    delete shapeeditor_;
    shapeeditor_ = new LineEditor();
}

void ShapeObjectsEditor::StartPointEditor(HWND hwnd, UINT checkItem)
{
    current = CurrentPaintingFigure::DOT;
    delete shape_;
    shape_ = new PointShape;
    delete shapeeditor_;
    shapeeditor_ = new PointEditor();
}

void ShapeObjectsEditor::StartRectEditor(HWND hwnd, UINT checkItem)
{
    current = CurrentPaintingFigure::RECTANGLE;
    delete shape_; 
    shape_ = new RectangleShape; 
    delete shapeeditor_; 
    shapeeditor_ = new RectEditor();  
}

void ShapeObjectsEditor::StartEllipseEditor(HWND hwnd, UINT checkItem)
{
    current = CurrentPaintingFigure::ELLIPSE;
    delete shape_;
    shape_ = new EllipseShape;
    delete shapeeditor_;
    shapeeditor_ = new EllipseEditor();
}

void ShapeObjectsEditor::OnPaint(HWND hwnd)
{
    HDC hdc = GetDC(hwnd); 
    for (std::uint32_t i = 0; i < countObjects; i++)
    {
        if (pchape[i] != nullptr)
        {
            pchape[i]->Show(hdc); 
            pchape[i] = nullptr;
        }
    }
    ReleaseDC(hwnd, hdc);  
}

void ShapeObjectsEditor::OnMouseMove(HWND hwnd, LPARAM lParam)
{
    if (current != CurrentPaintingFigure::NONE) {
        shapeeditor_->OnMouseMove(hwnd, lParam);
    }
}

void ShapeObjectsEditor::OnLBup(HWND hwnd, LPARAM lParam)
{
    if (current == CurrentPaintingFigure::RECTANGLE)
    {
        shapeeditor_->OnLBup(hwnd, lParam);
        
        shape_->Set(dynamic_cast<RectEditor*>(shapeeditor_)->startPoint.x, dynamic_cast<RectEditor*>(shapeeditor_)->startPoint.y, dynamic_cast<RectEditor*>(shapeeditor_)->endPoint.x, dynamic_cast<RectEditor*>(shapeeditor_)->endPoint.y);
        pchape[countObjects++] = shape_;
        InvalidateRect(hwnd, NULL, TRUE);  
    }
    else if (current == CurrentPaintingFigure::ELLIPSE)
    {
        shapeeditor_->OnLBup(hwnd, lParam); 
        shape_->Set(dynamic_cast<EllipseEditor*>(shapeeditor_)->startPoint.x, dynamic_cast<EllipseEditor*>(shapeeditor_)->startPoint.y, dynamic_cast<EllipseEditor*>(shapeeditor_)->endPoint.x, dynamic_cast<EllipseEditor*>(shapeeditor_)->endPoint.y);
        pchape[countObjects++] = shape_;
        InvalidateRect(hwnd, NULL, TRUE);
    }
    else if (current == CurrentPaintingFigure::LINE)
    {
        shapeeditor_->OnLBup(hwnd, lParam); 
        shape_->Set(dynamic_cast<LineEditor*>(shapeeditor_)->startPoint.x, dynamic_cast<LineEditor*>(shapeeditor_)->startPoint.y, dynamic_cast<LineEditor*>(shapeeditor_)->endPoint.x, dynamic_cast<LineEditor*>(shapeeditor_)->endPoint.y);
        pchape[countObjects++] = shape_;
        InvalidateRect(hwnd, NULL, TRUE);
    }
    else if (current == CurrentPaintingFigure::DOT)
    {
        shapeeditor_->OnLBup(hwnd, lParam);
    }
}

void ShapeObjectsEditor::OnLBDown(HWND hwnd, LPARAM lParam)
{
    if (current != CurrentPaintingFigure::NONE) {
        shapeeditor_->OnLBdown(hwnd, lParam);
    }
}