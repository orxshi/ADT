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

vtkNew<vtkNamedColors> colors;
vtkNew<vtkRenderer> renderer;

struct Polygon
{
    vtkSmartPointer<vtkPolyData> polydata;
    vtkSmartPointer<vtkPoints> points;
    vtkSmartPointer<vtkCellArray> polygons;
    vtkSmartPointer<vtkPolyDataMapper> pdt_mapper;

    Polygon()
    {
        polydata = vtkSmartPointer<vtkPolyData>::New();
        points = vtkSmartPointer<vtkPoints>::New();
        polygons = vtkSmartPointer<vtkCellArray>::New();
        pdt_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();

        read();
        polydata->SetPoints(points);
        polydata->SetPolys(polygons);
        pdt_mapper->SetInputData(polydata);

        vtkNew<vtkActor> actor_poly;
        actor_poly->SetMapper(pdt_mapper);
        actor_poly->GetProperty()->SetColor(colors->GetColor3d("Black").GetData());
        actor_poly->GetProperty()->SetRepresentationToWireframe();

        renderer->AddActor(actor_poly);
    }

    void make_quad(double xmin, double ymin, double xmax, double ymax)
    {
        vtkNew<vtkPolygon> polygon;

        points->InsertNextPoint(xmin, ymin, 0.0);
        points->InsertNextPoint(xmax, ymin, 0.0);
        points->InsertNextPoint(xmax, ymax, 0.0);
        points->InsertNextPoint(xmin, ymax, 0.0);

        auto size = points->GetNumberOfPoints();

        polygon->GetPointIds()->SetNumberOfIds(4);
        polygon->GetPointIds()->SetId(0, size-4);
        polygon->GetPointIds()->SetId(1, size-3);
        polygon->GetPointIds()->SetId(2, size-2);
        polygon->GetPointIds()->SetId(3, size-1);

        polygons->InsertNextCell(polygon);
    }

    void read()
    {
        std::ifstream in;
        in.open("../test/quad.txt");

        double xmin;
        double ymin;
        double xmax;
        double ymax;

        while(true)
        {
            in >> xmin;
            in >> ymin;
            in >> xmax;
            in >> ymax;

            if (in.eof())
            {
                in.close();
                break;
            }

            make_quad(xmin, ymin, xmax, ymax);
        }
    }
};

struct Vertex
{
    int nvertex;
    vtkSmartPointer<vtkIntArray> cell_name;
    vtkSmartPointer<vtkPoints> points;
    vtkSmartPointer<vtkCellArray> vertices;
    vtkSmartPointer<vtkPolyData> polydata;
    vtkSmartPointer<vtkPolyDataMapper> pdt_mapper;

    Vertex()
    {
        cell_name = vtkSmartPointer<vtkIntArray>::New();
        points = vtkSmartPointer<vtkPoints>::New();
        vertices = vtkSmartPointer<vtkCellArray>::New();
        polydata = vtkSmartPointer<vtkPolyData>::New();
        pdt_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();

        get_nvertex();
        cell_name->SetNumberOfValues(nvertex);
        cell_name->SetName("Name");

        read();
        polydata->SetPoints(points);
        polydata->SetVerts(vertices);

        polydata->GetCellData()->AddArray(cell_name);

        vtkNew<vtkLookupTable> lut;
        lut->SetNumberOfTableValues(1);
        lut->SetTableValue(0, 0, 0, 0, 1);
        lut->Build();

        pdt_mapper->SetInputData(polydata);
        pdt_mapper->SetLookupTable(lut);
        pdt_mapper->SetScalarVisibility(1);
        pdt_mapper->SetScalarModeToUseCellData();
        pdt_mapper->GetInput()->GetCellData()->SetActiveScalars("Name");

        vtkNew<vtkActor> actor_vert;
        actor_vert->SetMapper(pdt_mapper);
        actor_vert->GetProperty()->SetPointSize(10);
        actor_vert->GetProperty()->SetColor(colors->GetColor3d("Black").GetData());

        vtkNew<vtkCellCenters> cc;
        cc->SetInputData(polydata);

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

    void get_nvertex()
    {
        std::ifstream in;
        in.open("../test/vertex.txt");

        int id;
        double x;
        double y;
        double temp;

        nvertex = 0;
        while(true)
        {
            in >> id;
            in >> x;
            in >> y;
            in >> temp;
            in >> temp;

            if (in.eof())
            {
                in.close();
                break;
            }

            ++nvertex;
        }
    }

    void read()
    {
        std::ifstream in;
        in.open("../test/vertex.txt");

        int id;
        double x;
        double y;
        double temp;

        while(true)
        {
            in >> id;
            in >> x;
            in >> y;
            in >> temp;
            in >> temp;

            if (in.eof())
            {
                in.close();
                break;
            }

            make_vertex(id, x, y);
        }
    }

    void make_vertex(int id, double x, double y)
    {
        vtkNew<vtkVertex> vertex;

        points->InsertNextPoint(x, y, 0.0);

        auto size = points->GetNumberOfPoints();

        vertex->GetPointIds()->SetId(0, size - 1);

        vertices->InsertNextCell(vertex);

        cell_name->SetValue(size - 1, id);
    }
};

int main(int, char*[])
{
    Vertex vertex;
    Polygon polygon;

    vtkNew<vtkRenderWindow> renderWindow;
    renderWindow->AddRenderer(renderer);
    vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;
    renderWindowInteractor->SetRenderWindow(renderWindow);

    renderer->SetBackground(colors->GetColor3d("White").GetData());
    renderWindow->SetSize(1000, 1000);
    renderWindow->Render();
    //renderWindowInteractor->Start();

    // Screenshot
    vtkNew<vtkWindowToImageFilter> windowToImageFilter;
    windowToImageFilter->SetInput(renderWindow);
    windowToImageFilter->SetScale(1);
    windowToImageFilter->ReadFrontBufferOff();
    windowToImageFilter->Update();

    vtkNew<vtkPNGWriter> writer;
    writer->SetFileName("2dpoint.png");
    writer->SetInputConnection(windowToImageFilter->GetOutputPort());
    writer->Write();

    return EXIT_SUCCESS;
}
