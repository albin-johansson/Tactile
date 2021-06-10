#pragma once

#include "dock_widget.hpp"
#include "forward_declare.hpp"

TACTILE_FORWARD_DECLARE(tactile, ToolWidget)

namespace tactile {

class ToolDock final : public DockWidget
{
  Q_OBJECT

 public:
  explicit ToolDock(QWidget* parent = nullptr);

  void EnableTools();

  void DisableTools();

 public slots:
  void OnEnableStamp();
  void OnEnableEraser();
  void OnEnableBucket();

 signals:
  void S_EnabledStamp();
  void S_EnabledBucket();
  void S_EnabledEraser();

 private:
  ToolWidget* mWidget{};
};

}  // namespace tactile
