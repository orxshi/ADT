#include <vtkActor.h>
#include <vtkCellArray.h>
#include <vtkNamedColors.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkPolygon.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkExtractEdges.h>
#include <vtkVertex.h>
#include <vtkTextActor.h>
#include <vtkTextProperty.h>
#include <vtkTextMapper.h>
#include <vtkCoordinate.h>
#include <vtkCellData.h>
#include <vtkPointData.h>
#include <vtkLabeledDataMapper.h>
#include <vtkDataSetMapper.h>
#include <vtkCellCenters.h>
#include <vtkDoubleArray.h>
#include <vtkLookupTable.h>
#include <vtkWindowToImageFilter.h>
#include <vtkPNGWriter.h>
#include <vtkHexahedron.h>
#include <vtkAxesActor.h>
#include <vtkTransform.h>
#include <vtkUnstructuredGrid.h>


vtkNew<vtkNamedColors> colors;
vtkNew<vtkRenderer> renderer;

struct Polygon
{
    vtkSmartPointer<vtkUnstructuredGrid> ugrid;
    vtkSmartPointer<vtkPoints> points;
    vtkSmartPointer<vtkCellArray> polygons;
    vtkSmartPointer<vtkDataSetMapper> pdt_mapper;

    Polygon()
    {
        ugrid = vtkSmartPointer<vtkUnstructuredGrid>::New();
        points = vtkSmartPointer<vtkPoints>::New();
        polygons = vtkSmartPointer<vtkCellArray>::New();
        pdt_mapper = vtkSmartPointer<vtkDataSetMapper>::New();

        read();

        vtkNew<vtkExtractEdges> edges;
        edges->SetInputData(ugrid);

        pdt_mapper->SetInputConnection(edges->GetOutputPort());

        vtkNew<vtkActor> actor_poly;
        actor_poly->SetMapper(pdt_mapper);
        actor_poly->GetProperty()->SetColor(colors->GetColor3d("Red").GetData());

        renderer->AddActor(actor_poly);
    }

    void make_quad(double xmin, double ymin, double zmin, double xmax, double ymax, double zmax)
    {
        vtkNew<vtkHexahedron> hex;

        points->InsertNextPoint(xmin, ymin, zmin);
        points->InsertNextPoint(xmax, ymin, zmin);
        points->InsertNextPoint(xmax, ymax, zmin);
        points->InsertNextPoint(xmin, ymax, zmin);

        points->InsertNextPoint(xmin, ymin, zmax);
        points->InsertNextPoint(xmax, ymin, zmax);
        points->InsertNextPoint(xmax, ymax, zmax);
        points->InsertNextPoint(xmin, ymax, zmax);

        auto npoint = points->GetNumberOfPoints();

        int nvertex = 8;
        for (int i=0; i<nvertex; ++i)
        {
            hex->GetPointIds()->SetId(i, npoint - nvertex + i);
        }

        ugrid->SetPoints(points);
        ugrid->InsertNextCell(hex->GetCellType(), hex->GetPointIds());
    }

    void read()
    {
        std::ifstream in;
        in.open("../test/quad.txt");

        double xmin;
        double ymin;
        double zmin;
        double xmax;
        double ymax;
        double zmax;

        while(true)
        {
            in >> xmin;
            in >> ymin;
            in >> zmin;
            in >> xmax;
            in >> ymax;
            in >> zmax;

            if (in.eof())
            {
                in.close();
                break;
            }

            make_quad(xmin, ymin, zmin, xmax, ymax, zmax);
        }
    }
};

struct Object
{
    int nobject;
    vtkSmartPointer<vtkIntArray> cell_name;
    vtkSmartPointer<vtkPoints> points;
    vtkSmartPointer<vtkCellArray> cells;
    vtkSmartPointer<vtkUnstructuredGrid> ugrid;
    vtkSmartPointer<vtkDataSetMapper> pdt_mapper;

    Object()
    {
        cell_name = vtkSmartPointer<vtkIntArray>::New();
        points = vtkSmartPointer<vtkPoints>::New();
        cells = vtkSmartPointer<vtkCellArray>::New();
        ugrid = vtkSmartPointer<vtkUnstructuredGrid>::New();
        pdt_mapper = vtkSmartPointer<vtkDataSetMapper>::New();

        get_nobject();
        cell_name->SetNumberOfValues(nobject);
        cell_name->SetName("Name");

        read();

        ugrid->GetCellData()->AddArray(cell_name);

        vtkNew<vtkLookupTable> lut;
        lut->SetNumberOfTableValues(1);
        lut->SetTableValue(0, colors->GetColor4d("Black").GetData());
        lut->Build();

        pdt_mapper->SetLookupTable(lut);
        pdt_mapper->SetInputData(ugrid);
        pdt_mapper->SetScalarVisibility(1);
        pdt_mapper->SetScalarModeToUseCellData();
        pdt_mapper->GetInput()->GetCellData()->SetActiveScalars("Name");

        vtkNew<vtkActor> actor_vert;
        actor_vert->SetMapper(pdt_mapper);
        //actor_vert->GetProperty()->SetRepresentationToSurface();
        actor_vert->GetProperty()->SetRepresentationToWireframe();
        actor_vert->GetProperty()->EdgeVisibilityOn();
        actor_vert->GetProperty()->SetEdgeColor(1,0,0);
        actor_vert->GetProperty()->SetPointSize(10);
        actor_vert->GetProperty()->SetColor(colors->GetColor3d("Black").GetData());

        vtkNew<vtkCellCenters> cc;
        cc->SetInputData(ugrid);

        vtkNew<vtkLabeledDataMapper> label_mapper;
        label_mapper->SetInputConnection(cc->GetOutputPort());
        label_mapper->SetLabelModeToLabelScalars();
        label_mapper->GetLabelTextProperty()->SetFontSize(30);
        label_mapper->GetLabelTextProperty()->SetColor(0,0,0);

        vtkNew<vtkActor2D> actor_label;
        actor_label->SetMapper(label_mapper);

        renderer->AddActor(actor_vert);
        renderer->AddActor(actor_label);
    }

    void get_nobject()
    {
        std::ifstream in;
        in.open("../test/vertex.txt");

        int id;
        double xmin;
        double ymin;
        double zmin;
        double xmax;
        double ymax;
        double zmax;

        nobject = 0;
        while(true)
        {
            in >> id;
            in >> xmin;
            in >> ymin;
            in >> zmin;
            in >> xmax;
            in >> ymax;
            in >> zmax;

            if (in.eof())
            {
                in.close();
                break;
            }

            ++nobject;
        }
    }

    void read()
    {
        std::ifstream in;
        in.open("../test/vertex.txt");

        int id;
        double xmin;
        double ymin;
        double zmin;
        double xmax;
        double ymax;
        double zmax;

        while(true)
        {
            in >> id;
            in >> xmin;
            in >> ymin;
            in >> zmin;
            in >> xmax;
            in >> ymax;
            in >> zmax;

            if (in.eof())
            {
                in.close();
                break;
            }

            make_object(id, xmin, ymin, zmin, xmax, ymax, zmax);
        }
    }

    void make_object(int id, double xmin, double ymin, double zmin, double xmax, double ymax, double zmax)
    {
        points->InsertNextPoint(xmin, ymin, zmin);
        points->InsertNextPoint(xmax, ymin, zmin);
        points->InsertNextPoint(xmax, ymax, zmin);
        points->InsertNextPoint(xmin, ymax, zmin);

        points->InsertNextPoint(xmin, ymin, zmax);
        points->InsertNextPoint(xmax, ymin, zmax);
        points->InsertNextPoint(xmax, ymax, zmax);
        points->InsertNextPoint(xmin, ymax, zmax);

        ugrid->SetPoints(points);

        auto npoint = points->GetNumberOfPoints();

        int nvertex;
        vtkSmartPointer<vtkObject> hex;

        if (xmin == xmax && ymin == ymax && zmin == zmax)
        {
            nvertex = 1;
            vtkNew<vtkVertex> hex;

            for (int i=0; i<nvertex; ++i)
            {
                hex->GetPointIds()->SetId(i, npoint - nvertex + i);
            }

            ugrid->InsertNextCell(hex->GetCellType(), hex->GetPointIds());
        }
        else
        {
            nvertex = 8;
            vtkNew<vtkHexahedron> hex;

            for (int i=0; i<nvertex; ++i)
            {
                hex->GetPointIds()->SetId(i, npoint - nvertex + i);
            }

            ugrid->InsertNextCell(hex->GetCellType(), hex->GetPointIds());
        }

        auto ncell = ugrid->GetNumberOfCells();
        cell_name->SetValue(ncell - 1, id);
    }
};

int main(int, char*[])
{
    Object object;
    Polygon polygon;

    vtkNew<vtkRenderWindow> renderWindow;
    renderWindow->AddRenderer(renderer);
    vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;
    renderWindowInteractor->SetRenderWindow(renderWindow);

    //vtkNew<vtkTransform> transform;
    //transform->Translate(1.0, 0.0, 0.0);
    //vtkNew<vtkAxesActor> axes;
    //axes->SetUserTransform(transform);
    //renderer->AddActor(axes);

    renderer->SetBackground(colors->GetColor3d("White").GetData());
    renderWindow->SetSize(1000, 1000);
    renderWindow->Render();
    renderWindowInteractor->Start();

    // Screenshot
    vtkNew<vtkWindowToImageFilter> windowToImageFilter;
    windowToImageFilter->SetInput(renderWindow);
    windowToImageFilter->SetScale(1);
    //windowToImageFilter->ReadFrontBufferOff();
    windowToImageFilter->Update();

    vtkNew<vtkPNGWriter> writer;
    writer->SetFileName("result.png");
    writer->SetInputConnection(windowToImageFilter->GetOutputPort());
    writer->Write();

    return EXIT_SUCCESS;
}
