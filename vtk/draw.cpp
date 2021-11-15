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
#include <vtkCamera.h>
#include <vtkCaptionActor2D.h>
#include <filesystem>


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
        in.open("../test/quad.dat");

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
    int dim;
    int nobject;
    vtkSmartPointer<vtkIntArray> cell_name;
    vtkSmartPointer<vtkPoints> points;
    vtkSmartPointer<vtkCellArray> cells;
    vtkSmartPointer<vtkUnstructuredGrid> ugrid;
    vtkSmartPointer<vtkDataSetMapper> pdt_mapper;
    vtkSmartPointer<vtkActor> actor_vert;
    vtkSmartPointer<vtkActor2D> actor_label;

    Object(int dim, std::string filename): dim(dim)
    {
        cell_name = vtkSmartPointer<vtkIntArray>::New();
        points = vtkSmartPointer<vtkPoints>::New();
        cells = vtkSmartPointer<vtkCellArray>::New();
        ugrid = vtkSmartPointer<vtkUnstructuredGrid>::New();
        pdt_mapper = vtkSmartPointer<vtkDataSetMapper>::New();
        actor_vert = vtkSmartPointer<vtkActor>::New();
        actor_label = vtkSmartPointer<vtkActor2D>::New();

        get_nobject(filename);
        cell_name->SetNumberOfValues(nobject);
        cell_name->SetName("Name");

        read(filename);

        ugrid->GetCellData()->AddArray(cell_name);
        ugrid->GetCellData()->SetActiveScalars("Name");

        if (dim == 0)
        {
            pdt_mapper->SetInputData(ugrid);
        }
        else
        {
            vtkNew<vtkExtractEdges> edges;
            edges->SetInputData(ugrid);

            pdt_mapper->SetInputConnection(edges->GetOutputPort());
        }

        pdt_mapper->SetScalarVisibility(0);

        actor_vert->SetMapper(pdt_mapper);
        actor_vert->GetProperty()->SetColor(colors->GetColor3d("Black").GetData());
        actor_vert->GetProperty()->SetPointSize(10);

        vtkNew<vtkCellCenters> cc;
        cc->SetInputData(ugrid);

        vtkNew<vtkLabeledDataMapper> label_mapper;
        label_mapper->SetInputConnection(cc->GetOutputPort());
        label_mapper->SetLabelModeToLabelScalars();
        label_mapper->GetLabelTextProperty()->SetFontSize(30);
        label_mapper->GetLabelTextProperty()->SetColor(0,0,0);

        actor_label->SetMapper(label_mapper);

        renderer->AddActor(actor_vert);
        renderer->AddActor(actor_label);
    }

    void get_nobject(std::string filename)
    {
        std::ifstream in;
        in.open(filename);

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

    void read(std::string filename)
    {
        std::ifstream in;
        in.open(filename);

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

        //if (xmin == xmax && ymin == ymax && zmin == zmax)
        if (dim == 0)
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

void set_axis(vtkCaptionActor2D* actor)
{
    actor->GetCaptionTextProperty()->SetColor(0,0,0);
    actor->GetTextActor()->SetTextScaleModeToNone();
    actor->GetCaptionTextProperty()->SetFontSize(30);
}

vtkSmartPointer<vtkAxesActor> set_axes()
{
    vtkNew<vtkTransform> transform;
    transform->Translate(1.1, 0.0, 0.0);

    vtkNew<vtkAxesActor> axes;
    axes->SetTotalLength(0.1,0.1,0.1);
    axes->SetUserTransform(transform);

    set_axis(axes->GetXAxisCaptionActor2D());
    set_axis(axes->GetYAxisCaptionActor2D());
    set_axis(axes->GetZAxisCaptionActor2D());

    return axes;
}

int main(int argc, char* argv[])
{
    assert(argc == 2);
    int dim = atoi(argv[1]);

    Object object(dim, "../test/vertex.dat");
    Polygon polygon;

    if (std::filesystem::exists("../test/search.dat"))
    {
        Object search(dim, "../test/search.dat");
        search.actor_vert->GetProperty()->SetColor(colors->GetColor3d("Green").GetData());

        Object target(dim, "../test/target.dat");
        target.actor_vert->GetProperty()->SetColor(colors->GetColor3d("Blue").GetData());
        renderer->RemoveActor(target.actor_label);
    }

    vtkNew<vtkRenderWindow> renderWindow;
    renderWindow->AddRenderer(renderer);
    vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;
    renderWindowInteractor->SetRenderWindow(renderWindow);

    auto axes = set_axes();
    renderer->AddActor(axes);

    //renderer->ResetCamera();
    //renderer->GetActiveCamera()->Azimuth(210);
    //renderer->GetActiveCamera()->Elevation(30);
    //renderer->ResetCameraClippingRange();

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
