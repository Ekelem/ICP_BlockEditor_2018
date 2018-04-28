#ifndef MYSCROLLAREA
#define MYSCROLLAREA

#include <QFrame>
#include <QLabel>
#include <QLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QVector>
#include <QMainWindow>
#include <QAction>
#include <QActionGroup>
#include <QMenu>
#include <QMenuBar>
#include <QDialogButtonBox>
#include <QGroupBox>
#include <QLineEdit>

class QDragEnterEvent;
class QDropEvent;

enum setMode { OBJECTS_ARE_DRAGGABLE, OBJECTS_ARE_CONNECTABLE };



class MyArea: public QFrame {
    Q_OBJECT

public:
    MyArea(QWidget *parent = nullptr, QMainWindow *s = nullptr);
    ~MyArea();
    void initialize();
    void create_block();
    void setObjectsToMoveable(bool isMoveAble);
    void setActualBlock(int type);

    bool openImage(const QString &fileName);
    bool saveImage(const QString &fileName, const char *fileFormat);
    void setPenColor(const QColor &newColor);
    void setPenWidth(int newWidth);
    bool isModified() const { return modified; }
    QColor penColor() const { return myPenColor; }
    int penWidth() const { return myPenWidth; }
    double blockValue = 0;

private:
    QMainWindow *p;
    QVBoxLayout *layout;
    QHBoxLayout *menu;
    QLabel *source;
    QLabel *destination;
    QGroupBox *groupModeBox;
    int mode = OBJECTS_ARE_DRAGGABLE;
    QPoint drawFrom,drawTo;
    bool modified;
    bool scribbling;
    int myPenWidth;
    QColor myPenColor;
    QImage image;
    int actualBlockType; /*add, sub, value,...*/

    void createObjects();
    void drawLineTo(const QPoint &endPoint);
    void resizeImage(QImage *image, const QSize &newSize);

private slots:
    void clearImage();


protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;


};


#endif // MYSCROLLAREA

