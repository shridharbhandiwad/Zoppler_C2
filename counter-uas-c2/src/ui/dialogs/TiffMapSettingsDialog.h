#ifndef TIFFMAPSETTINGSDIALOG_H
#define TIFFMAPSETTINGSDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QSlider>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>
#include "ui/MapWidget.h"

namespace CounterUAS {

/**
 * TiffMapSettingsDialog - Dialog for configuring TIFF map settings
 * 
 * Allows users to:
 * - Load a TIFF/image file
 * - Set geo-referenced bounds (north, south, east, west)
 * - Adjust opacity
 * - Toggle visibility
 */
class TiffMapSettingsDialog : public QDialog {
    Q_OBJECT
    
public:
    explicit TiffMapSettingsDialog(MapWidget* mapWidget, QWidget* parent = nullptr);
    
    void setCurrentBounds(const TiffMapBounds& bounds);
    TiffMapBounds bounds() const;
    
    void setFilePath(const QString& path);
    QString filePath() const;
    
    void setOpacity(double opacity);
    double opacity() const;
    
signals:
    void mapFileSelected(const QString& filePath, const TiffMapBounds& bounds);
    void boundsChanged(const TiffMapBounds& bounds);
    void opacityChanged(double opacity);
    void visibilityChanged(bool visible);
    void mapCleared();
    
private slots:
    void onBrowseClicked();
    void onLoadClicked();
    void onClearClicked();
    void onCenterOnMapClicked();
    void onOpacityChanged(int value);
    void onBoundsEdited();
    void onVisibilityToggled(bool checked);
    void updatePreview();
    
private:
    void setupUI();
    void updateBoundsFromFile();
    bool validateBounds();
    void applyDarkTheme();
    
    MapWidget* m_mapWidget;
    
    // File selection
    QLineEdit* m_filePathEdit;
    QPushButton* m_browseBtn;
    QPushButton* m_loadBtn;
    QPushButton* m_clearBtn;
    
    // Bounds inputs
    QDoubleSpinBox* m_northLatSpin;
    QDoubleSpinBox* m_southLatSpin;
    QDoubleSpinBox* m_eastLonSpin;
    QDoubleSpinBox* m_westLonSpin;
    
    // Display options
    QSlider* m_opacitySlider;
    QLabel* m_opacityLabel;
    QCheckBox* m_visibilityCheck;
    
    // Preview and info
    QLabel* m_previewLabel;
    QLabel* m_infoLabel;
    
    // Navigation
    QPushButton* m_centerOnMapBtn;
};

} // namespace CounterUAS

#endif // TIFFMAPSETTINGSDIALOG_H
