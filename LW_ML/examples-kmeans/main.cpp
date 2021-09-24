#include <opencv2/core/core.hpp>

using namespace cv;

Mat generateDataset()
{
    int n = 300;
    Mat data(3 * n, 2, CV_32F);
    
    randn(data(Range(0, n), Range(0, 1)), 0.0, 0.05);
    randn(data(Range(0, n), Range(1, 2)), 0.5, 0.25);

    randn(data(Range(n, 2 * n), Range(0, 1)), 0.7, 0.25);
    randn(data(Range(n, 2 * n), Range(1, 2)), 0.0, 0.05);

    randn(data(Range(2 * n, 3 * n), Range(0, 1)),
          0.7, 0.15);
    randn(data(Range(2 * n, 3 * n), Range(1, 2)),
          0.8, 0.15);
    
    return data;
}

int main(int argc, char* argv[])
{
    Mat samples = generateDataset();

    Mat labels;
    Mat centers;
    kmeans(samples,
        3,
        labels,
        TermCriteria(TermCriteria::COUNT +
                     TermCriteria::EPS, 10000, 0.001),
        10,
        KMEANS_PP_CENTERS,
        centers);
    
    return 0;
}
