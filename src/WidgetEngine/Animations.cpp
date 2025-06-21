#include <WidgetEngine/WidgetEngine.h>

namespace WidgetEngine {
    void Engine::AddAnimation(const std::string& window, const std::string& widget, const std::string& name, const std::string& property, unsigned int duration, AnimCurve curve) {
      WindowHandle* win = GetWindow(window);
      if (win && win->window && win->window->isWindow()) {
        if (auto* w = win->window->findChild<QWidget*>(QString::fromStdString(widget))) {
          std::shared_ptr<QPropertyAnimation> anim(new QPropertyAnimation(w, property.c_str()));
          anim->setDuration(duration);
          anim->setEasingCurve(QEasingCurve::Type(curve));

          animations[name] = std::move(anim);
        }
      }
    }

  void Engine::AnimateWidgetGeometryOnce(const std::string& window, const std::string& name, QRect start, QRect end, unsigned int duration, AnimCurve curve) {
    WindowHandle* win = GetWindow(window);
    if (win && win->window && win->window->isWindow()) {
      if (auto* w = win->window->findChild<QWidget*>(QString::fromStdString(name))) {
        w->setGeometry(start);
        QPropertyAnimation* anim = new QPropertyAnimation(w, "geometry");
        QObject::connect(anim, &QPropertyAnimation::finished, anim, &QPropertyAnimation::deleteLater);
        anim->setDuration(duration);
        anim->setEasingCurve(QEasingCurve::Type(curve));
        anim->setStartValue(start);
        anim->setEndValue(end);
        anim->start();
      }
    }
  }

  void Engine::AnimateWidgetMaxSizeOnce(const std::string& window, const std::string& name, QSize start, QSize end, unsigned int duration, AnimCurve curve) {
    WindowHandle* win = GetWindow(window);
    if (win && win->window && win->window->isWindow()) {
      if (auto* w = win->window->findChild<QWidget*>(QString::fromStdString(name))) {
        w->setMaximumSize(start);
        QPropertyAnimation* anim = new QPropertyAnimation(w, "maximumSize");
        QObject::connect(anim, &QPropertyAnimation::finished, anim, &QPropertyAnimation::deleteLater);
        anim->setDuration(duration);
        anim->setEasingCurve(QEasingCurve::Type(curve));
        anim->setStartValue(start);
        anim->setEndValue(end);
        anim->start();
      }
    }
  }

  void Engine::AnimateWidgetMinSizeOnce(const std::string& window, const std::string& name, QSize start, QSize end, unsigned int duration, AnimCurve curve) {
    WindowHandle* win = GetWindow(window);
    if (win && win->window && win->window->isWindow()) {
      if (auto* w = win->window->findChild<QWidget*>(QString::fromStdString(name))) {
        w->setMinimumSize(start);
        QPropertyAnimation* anim = new QPropertyAnimation(w, "minimumSize");
        QObject::connect(anim, &QPropertyAnimation::finished, anim, &QPropertyAnimation::deleteLater);
        anim->setDuration(duration);
        anim->setEasingCurve(QEasingCurve::Type(curve));
        anim->setStartValue(start);
        anim->setEndValue(end);
        anim->start();
      }
    }
  }

  void Engine::SetAnimStartValue(const std::string& animation, const QVariant& value) {
    auto it = animations.find(animation);
    if (it != animations.end()) {
      it->second->setStartValue(value);
    } else {
      if (BL_DEBUG) {
        std::clog << "Animation not found: " << animation << std::endl;
      }
    }
  }

  bool Engine::AnimationPlaying(const std::string& animation) {
    auto it = animations.find(animation);
    if (it != animations.end()) {
      return it->second->state() == QAbstractAnimation::State::Running;
    }
    return false;
  }

  void Engine::SetAnimEndValue(const std::string& animation, const QVariant& value) {
    auto it = animations.find(animation);
    if (it != animations.end()) {
      it->second->setEndValue(value);
    } else {
      if (BL_DEBUG) {
        std::clog << "Animation not found: " << animation << std::endl;
      }
    }
  }

  void Engine::StartAnimation(const std::string& animation) {
    auto it = animations.find(animation);
    if (it != animations.end()) {
      it->second->start();
    }
  }

  void Engine::StopAnimation(const std::string& animation) {
    auto it = animations.find(animation);
    if (it != animations.end()) {
      it->second->stop();
    }
  }

  void Engine::SetAnimStartValueRect(const std::string& animation, unsigned int x, unsigned int y, unsigned int width, unsigned int height) {
    auto it = animations.find(animation);
    if (it != animations.end()) {
      it->second->setStartValue(QRect(x, y, width, height));
    }
  }

  void Engine::SetAnimEndValueRect(const std::string& animation, unsigned int x, unsigned int y, unsigned int width, unsigned int height) {
    auto it = animations.find(animation);
    if (it != animations.end()) {
      it->second->setEndValue(QRect(x, y, width, height));
    }
  }

  void Engine::SetAnimDirection(const std::string& animation, bool forward) {
    auto it = animations.find(animation);
    if (it != animations.end()) {
      it->second->setDirection(forward ? QAbstractAnimation::Direction::Forward : QAbstractAnimation::Direction::Backward);
    }
  }
};
