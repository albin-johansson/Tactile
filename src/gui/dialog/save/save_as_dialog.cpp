#include "save_as_dialog.hpp"

#include "preferences.hpp"
#include "standard_paths.hpp"
#include "tactile_qstring.hpp"

namespace tactile {

SaveAsDialog::SaveAsDialog(const QString& fileName, QWidget* parent)
    : QFileDialog{parent}
{
  setAcceptMode(QFileDialog::AcceptSave);

  if (const auto format = prefs::DefaultFormat();
      format.Value() == TACTILE_QSTRING(u"JSON"))
  {
    setNameFilter(tr("JSON files (*.json);;TMX files (*.tmx)"));
  }
  else
  {
    setNameFilter(tr("TMX files (*.tmx);;JSON files (*.json)"));
  }

  setWindowTitle(tr("Save As..."));
  setDirectory(GetDocumentsPath());
  selectFile(fileName);
}

}  // namespace tactile
