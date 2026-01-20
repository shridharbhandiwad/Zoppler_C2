#include "ui/dialogs/TiffMapSettingsDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QFileDialog>
#include <QMessageBox>
#include <QFileInfo>
#include <QImageReader>

namespace CounterUAS {

TiffMapSettingsDialog::TiffMapSettingsDialog(MapWidget* mapWidget, QWidget* parent)
    : QDialog(parent)
    , m_mapWidget(mapWidget)
{
    setWindowTitle("TIFF Map Settings");
    setMinimumSize(500, 550);
    setModal(false);  // Allow interaction with map while dialog is open
    
    setupUI();
    applyDarkTheme();
    
    // Initialize with current map state if loaded
    if (m_mapWidget && m_mapWidget->hasTiffMap()) {
        m_filePathEdit->setText(m_mapWidget->tiffMapPath());
        setCurrentBounds(m_mapWidget->tiffMapBounds());
        m_opacitySlider->setValue(static_cast<int>(m_mapWidget->tiffMapOpacity() * 100));
        m_visibilityCheck->setChecked(m_mapWidget->showTiffMap());
        updatePreview();
    }
}

void TiffMapSettingsDialog::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(16);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    
    // ========== File Selection Group ==========
    QGroupBox* fileGroup = new QGroupBox("Map File", this);
    QVBoxLayout* fileLayout = new QVBoxLayout(fileGroup);
    
    QHBoxLayout* pathLayout = new QHBoxLayout();
    m_filePathEdit = new QLineEdit(this);
    m_filePathEdit->setPlaceholderText("Select a TIFF, PNG, or JPEG map file...");
    m_filePathEdit->setReadOnly(true);
    pathLayout->addWidget(m_filePathEdit, 1);
    
    m_browseBtn = new QPushButton("Browse...", this);
    m_browseBtn->setFixedWidth(100);
    connect(m_browseBtn, &QPushButton::clicked, this, &TiffMapSettingsDialog::onBrowseClicked);
    pathLayout->addWidget(m_browseBtn);
    
    fileLayout->addLayout(pathLayout);
    
    // Load/Clear buttons
    QHBoxLayout* actionLayout = new QHBoxLayout();
    m_loadBtn = new QPushButton("Load Map", this);
    m_loadBtn->setFixedHeight(32);
    connect(m_loadBtn, &QPushButton::clicked, this, &TiffMapSettingsDialog::onLoadClicked);
    actionLayout->addWidget(m_loadBtn);
    
    m_clearBtn = new QPushButton("Clear Map", this);
    m_clearBtn->setFixedHeight(32);
    connect(m_clearBtn, &QPushButton::clicked, this, &TiffMapSettingsDialog::onClearClicked);
    actionLayout->addWidget(m_clearBtn);
    
    fileLayout->addLayout(actionLayout);
    
    // Preview label
    m_previewLabel = new QLabel(this);
    m_previewLabel->setFixedHeight(100);
    m_previewLabel->setAlignment(Qt::AlignCenter);
    m_previewLabel->setStyleSheet("background-color: #1a2a3a; border: 1px solid #3a4a5a; border-radius: 4px;");
    m_previewLabel->setText("No map loaded");
    fileLayout->addWidget(m_previewLabel);
    
    mainLayout->addWidget(fileGroup);
    
    // ========== Geo Bounds Group ==========
    QGroupBox* boundsGroup = new QGroupBox("Geographic Bounds", this);
    QGridLayout* boundsLayout = new QGridLayout(boundsGroup);
    boundsLayout->setSpacing(10);
    
    // North
    QLabel* northLabel = new QLabel("North Latitude:", this);
    m_northLatSpin = new QDoubleSpinBox(this);
    m_northLatSpin->setRange(-90.0, 90.0);
    m_northLatSpin->setDecimals(6);
    m_northLatSpin->setSuffix("°");
    m_northLatSpin->setSingleStep(0.001);
    connect(m_northLatSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &TiffMapSettingsDialog::onBoundsEdited);
    boundsLayout->addWidget(northLabel, 0, 0);
    boundsLayout->addWidget(m_northLatSpin, 0, 1);
    
    // South
    QLabel* southLabel = new QLabel("South Latitude:", this);
    m_southLatSpin = new QDoubleSpinBox(this);
    m_southLatSpin->setRange(-90.0, 90.0);
    m_southLatSpin->setDecimals(6);
    m_southLatSpin->setSuffix("°");
    m_southLatSpin->setSingleStep(0.001);
    connect(m_southLatSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &TiffMapSettingsDialog::onBoundsEdited);
    boundsLayout->addWidget(southLabel, 1, 0);
    boundsLayout->addWidget(m_southLatSpin, 1, 1);
    
    // East
    QLabel* eastLabel = new QLabel("East Longitude:", this);
    m_eastLonSpin = new QDoubleSpinBox(this);
    m_eastLonSpin->setRange(-180.0, 180.0);
    m_eastLonSpin->setDecimals(6);
    m_eastLonSpin->setSuffix("°");
    m_eastLonSpin->setSingleStep(0.001);
    connect(m_eastLonSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &TiffMapSettingsDialog::onBoundsEdited);
    boundsLayout->addWidget(eastLabel, 0, 2);
    boundsLayout->addWidget(m_eastLonSpin, 0, 3);
    
    // West
    QLabel* westLabel = new QLabel("West Longitude:", this);
    m_westLonSpin = new QDoubleSpinBox(this);
    m_westLonSpin->setRange(-180.0, 180.0);
    m_westLonSpin->setDecimals(6);
    m_westLonSpin->setSuffix("°");
    m_westLonSpin->setSingleStep(0.001);
    connect(m_westLonSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &TiffMapSettingsDialog::onBoundsEdited);
    boundsLayout->addWidget(westLabel, 1, 2);
    boundsLayout->addWidget(m_westLonSpin, 1, 3);
    
    // Center on map button
    m_centerOnMapBtn = new QPushButton("Center View on Map", this);
    m_centerOnMapBtn->setFixedHeight(28);
    connect(m_centerOnMapBtn, &QPushButton::clicked, this, &TiffMapSettingsDialog::onCenterOnMapClicked);
    boundsLayout->addWidget(m_centerOnMapBtn, 2, 0, 1, 4);
    
    mainLayout->addWidget(boundsGroup);
    
    // ========== Display Options Group ==========
    QGroupBox* displayGroup = new QGroupBox("Display Options", this);
    QVBoxLayout* displayLayout = new QVBoxLayout(displayGroup);
    
    // Visibility checkbox
    m_visibilityCheck = new QCheckBox("Show TIFF Map", this);
    m_visibilityCheck->setChecked(true);
    connect(m_visibilityCheck, &QCheckBox::toggled, this, &TiffMapSettingsDialog::onVisibilityToggled);
    displayLayout->addWidget(m_visibilityCheck);
    
    // Opacity slider
    QHBoxLayout* opacityLayout = new QHBoxLayout();
    QLabel* opacityTitleLabel = new QLabel("Opacity:", this);
    opacityLayout->addWidget(opacityTitleLabel);
    
    m_opacitySlider = new QSlider(Qt::Horizontal, this);
    m_opacitySlider->setRange(0, 100);
    m_opacitySlider->setValue(100);
    connect(m_opacitySlider, &QSlider::valueChanged, this, &TiffMapSettingsDialog::onOpacityChanged);
    opacityLayout->addWidget(m_opacitySlider, 1);
    
    m_opacityLabel = new QLabel("100%", this);
    m_opacityLabel->setFixedWidth(50);
    m_opacityLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    opacityLayout->addWidget(m_opacityLabel);
    
    displayLayout->addLayout(opacityLayout);
    
    mainLayout->addWidget(displayGroup);
    
    // ========== Info Label ==========
    m_infoLabel = new QLabel(this);
    m_infoLabel->setWordWrap(true);
    m_infoLabel->setStyleSheet("color: #88aacc; font-size: 11px;");
    m_infoLabel->setText(
        "Tip: For accurate positioning, set the geographic bounds to match the "
        "coordinates of the map image corners. If a world file (.tfw, .tifw, .wld) "
        "exists alongside your TIFF, bounds will be auto-detected."
    );
    mainLayout->addWidget(m_infoLabel);
    
    // ========== Close Button ==========
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    
    QPushButton* closeBtn = new QPushButton("Close", this);
    closeBtn->setFixedSize(100, 32);
    connect(closeBtn, &QPushButton::clicked, this, &QDialog::accept);
    buttonLayout->addWidget(closeBtn);
    
    mainLayout->addLayout(buttonLayout);
}

void TiffMapSettingsDialog::applyDarkTheme() {
    setStyleSheet(R"(
        QDialog {
            background-color: #0d1a2d;
            color: #ffffff;
        }
        QGroupBox {
            font-weight: bold;
            color: #00d4ff;
            border: 1px solid #1a3344;
            border-radius: 6px;
            margin-top: 12px;
            padding-top: 10px;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            subcontrol-position: top left;
            padding: 0 8px;
            background-color: #0d1a2d;
        }
        QLineEdit {
            background-color: #111d2e;
            border: 1px solid #1a3344;
            border-radius: 4px;
            padding: 6px 10px;
            color: #ffffff;
        }
        QLineEdit:focus {
            border-color: #00d4ff;
        }
        QDoubleSpinBox {
            background-color: #111d2e;
            border: 1px solid #1a3344;
            border-radius: 4px;
            padding: 4px 8px;
            color: #ffffff;
        }
        QDoubleSpinBox:focus {
            border-color: #00d4ff;
        }
        QPushButton {
            background-color: #1a3344;
            color: #ffffff;
            border: 1px solid #2a4454;
            border-radius: 4px;
            padding: 6px 16px;
            font-weight: bold;
        }
        QPushButton:hover {
            background-color: #2a4454;
            border-color: #00d4ff;
        }
        QPushButton:pressed {
            background-color: #0a2030;
        }
        QSlider::groove:horizontal {
            height: 6px;
            background: #1a3344;
            border-radius: 3px;
        }
        QSlider::handle:horizontal {
            background: #00d4ff;
            width: 16px;
            height: 16px;
            margin: -5px 0;
            border-radius: 8px;
        }
        QSlider::sub-page:horizontal {
            background: #00d4ff;
            border-radius: 3px;
        }
        QCheckBox {
            color: #ffffff;
            spacing: 8px;
        }
        QCheckBox::indicator {
            width: 18px;
            height: 18px;
            border: 1px solid #1a3344;
            border-radius: 3px;
            background-color: #111d2e;
        }
        QCheckBox::indicator:checked {
            background-color: #00d4ff;
            border-color: #00d4ff;
        }
        QLabel {
            color: #ccddee;
        }
    )");
}

void TiffMapSettingsDialog::setCurrentBounds(const TiffMapBounds& bounds) {
    m_northLatSpin->blockSignals(true);
    m_southLatSpin->blockSignals(true);
    m_eastLonSpin->blockSignals(true);
    m_westLonSpin->blockSignals(true);
    
    m_northLatSpin->setValue(bounds.northLat);
    m_southLatSpin->setValue(bounds.southLat);
    m_eastLonSpin->setValue(bounds.eastLon);
    m_westLonSpin->setValue(bounds.westLon);
    
    m_northLatSpin->blockSignals(false);
    m_southLatSpin->blockSignals(false);
    m_eastLonSpin->blockSignals(false);
    m_westLonSpin->blockSignals(false);
}

TiffMapBounds TiffMapSettingsDialog::bounds() const {
    TiffMapBounds b;
    b.northLat = m_northLatSpin->value();
    b.southLat = m_southLatSpin->value();
    b.eastLon = m_eastLonSpin->value();
    b.westLon = m_westLonSpin->value();
    return b;
}

void TiffMapSettingsDialog::setFilePath(const QString& path) {
    m_filePathEdit->setText(path);
}

QString TiffMapSettingsDialog::filePath() const {
    return m_filePathEdit->text();
}

void TiffMapSettingsDialog::setOpacity(double opacity) {
    m_opacitySlider->setValue(static_cast<int>(opacity * 100));
}

double TiffMapSettingsDialog::opacity() const {
    return m_opacitySlider->value() / 100.0;
}

void TiffMapSettingsDialog::onBrowseClicked() {
    QString filter = "Image Files (*.tif *.tiff *.png *.jpg *.jpeg *.bmp);;TIFF Files (*.tif *.tiff);;All Files (*)";
    QString filePath = QFileDialog::getOpenFileName(this, "Select Map Image", QString(), filter);
    
    if (!filePath.isEmpty()) {
        m_filePathEdit->setText(filePath);
        updateBoundsFromFile();
        updatePreview();
    }
}

void TiffMapSettingsDialog::onLoadClicked() {
    QString path = m_filePathEdit->text();
    if (path.isEmpty()) {
        QMessageBox::warning(this, "No File Selected", "Please select a map file first.");
        return;
    }
    
    if (!validateBounds()) {
        return;
    }
    
    TiffMapBounds b = bounds();
    
    if (m_mapWidget) {
        bool success = m_mapWidget->loadTiffMap(path, b);
        if (success) {
            m_infoLabel->setText("Map loaded successfully!");
            m_infoLabel->setStyleSheet("color: #00ff88; font-size: 11px;");
            emit mapFileSelected(path, b);
        } else {
            m_infoLabel->setText("Failed to load map. Check the file format.");
            m_infoLabel->setStyleSheet("color: #ff3344; font-size: 11px;");
        }
    }
}

void TiffMapSettingsDialog::onClearClicked() {
    if (m_mapWidget) {
        m_mapWidget->clearTiffMap();
    }
    
    m_filePathEdit->clear();
    m_previewLabel->setPixmap(QPixmap());
    m_previewLabel->setText("No map loaded");
    
    // Reset bounds to default
    m_northLatSpin->setValue(0);
    m_southLatSpin->setValue(0);
    m_eastLonSpin->setValue(0);
    m_westLonSpin->setValue(0);
    
    m_infoLabel->setText("Map cleared.");
    m_infoLabel->setStyleSheet("color: #88aacc; font-size: 11px;");
    
    emit mapCleared();
}

void TiffMapSettingsDialog::onCenterOnMapClicked() {
    if (m_mapWidget && m_mapWidget->hasTiffMap()) {
        m_mapWidget->centerOnTiffMap();
    }
}

void TiffMapSettingsDialog::onOpacityChanged(int value) {
    m_opacityLabel->setText(QString("%1%").arg(value));
    
    if (m_mapWidget) {
        m_mapWidget->setTiffMapOpacity(value / 100.0);
    }
    
    emit opacityChanged(value / 100.0);
}

void TiffMapSettingsDialog::onBoundsEdited() {
    if (m_mapWidget && m_mapWidget->hasTiffMap()) {
        TiffMapBounds b = bounds();
        if (b.isValid()) {
            m_mapWidget->setTiffMapBounds(b);
            emit boundsChanged(b);
        }
    }
}

void TiffMapSettingsDialog::onVisibilityToggled(bool checked) {
    if (m_mapWidget) {
        m_mapWidget->setShowTiffMap(checked);
    }
    emit visibilityChanged(checked);
}

void TiffMapSettingsDialog::updatePreview() {
    QString path = m_filePathEdit->text();
    if (path.isEmpty()) {
        m_previewLabel->setPixmap(QPixmap());
        m_previewLabel->setText("No map loaded");
        return;
    }
    
    QImageReader reader(path);
    if (!reader.canRead()) {
        m_previewLabel->setText("Cannot read file");
        return;
    }
    
    QSize originalSize = reader.size();
    
    // Scale to fit preview area while maintaining aspect ratio
    QSize previewSize = m_previewLabel->size() - QSize(4, 4);
    reader.setScaledSize(originalSize.scaled(previewSize, Qt::KeepAspectRatio));
    
    QImage preview = reader.read();
    if (!preview.isNull()) {
        m_previewLabel->setPixmap(QPixmap::fromImage(preview));
        
        // Update info label with image details
        m_infoLabel->setText(QString("Image: %1 x %2 pixels")
                            .arg(originalSize.width())
                            .arg(originalSize.height()));
        m_infoLabel->setStyleSheet("color: #88aacc; font-size: 11px;");
    } else {
        m_previewLabel->setText("Failed to load preview");
    }
}

void TiffMapSettingsDialog::updateBoundsFromFile() {
    // This will be handled by MapWidget::tryParseGeoTiffBounds
    // For now, set default bounds based on current map center
    if (m_mapWidget) {
        GeoPosition center = m_mapWidget->center();
        double span = 0.05;  // Default span
        
        m_northLatSpin->setValue(center.latitude + span);
        m_southLatSpin->setValue(center.latitude - span);
        m_eastLonSpin->setValue(center.longitude + span);
        m_westLonSpin->setValue(center.longitude - span);
    }
}

bool TiffMapSettingsDialog::validateBounds() {
    TiffMapBounds b = bounds();
    
    if (!b.isValid()) {
        QString error;
        if (b.northLat <= b.southLat) {
            error = "North latitude must be greater than South latitude.";
        } else if (b.eastLon <= b.westLon) {
            error = "East longitude must be greater than West longitude.";
        } else {
            error = "Invalid geographic bounds.";
        }
        
        QMessageBox::warning(this, "Invalid Bounds", error);
        return false;
    }
    
    return true;
}

} // namespace CounterUAS
