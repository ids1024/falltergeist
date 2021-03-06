#include <memory>
#include "../Event/Event.h"
#include "../UI/Animation.h"
#include "../UI/AnimationQueue.h"

namespace Falltergeist
{
    namespace UI
    {
        AnimationQueue::AnimationQueue() : Falltergeist::UI::Base()
        {
        }

        AnimationQueue::~AnimationQueue()
        {
            clear();
        }

        std::vector<std::unique_ptr<Animation>>& AnimationQueue::animations()
        {
            return _animations;
        }

        void AnimationQueue::clear()
        {
            _currentAnimation = 0;
            _playing = false;
            _repeat = false;
            _animations.clear();
        }

        void AnimationQueue::stop()
        {
            _playing = false;
            _currentAnimation = 0;
            currentAnimation()->stop();
        }

        void AnimationQueue::start()
        {
            _playing = true;
        }

        void AnimationQueue::setRepeat(bool value)
        {
            _repeat = value;
        }

        void AnimationQueue::render(bool eggTransparency)
        {
            currentAnimation()->setLightLevel(_lightLevel);
            currentAnimation()->setLight(light());
            currentAnimation()->setTrans(_trans);
            currentAnimation()->setPosition(this->position());
            currentAnimation()->render(eggTransparency);
        }

        void AnimationQueue::think(const float &deltaTime)
        {
            if (_playing)
            {
                if (currentAnimation()->ended())
                {
                    // not last animation in queue
                    if (_currentAnimation < _animations.size() - 1)
                    {
                        _currentAnimation++;
                        currentAnimation()->stop(); // rewind
                        currentAnimation()->play();
                    }
                    else
                    {
                        if (!_repeat)
                        {
                            emitEvent(std::make_unique<Event::Event>("animationEnded"), animationEndedHandler());
                            currentAnimation()->stop();
                            _playing = false;
                            return;
                        }
                        else
                        {
                            _currentAnimation = 0;
                            currentAnimation()->stop();
                            currentAnimation()->play();
                        }
                    }
                }
                else if (!currentAnimation()->playing())
                {
                    currentAnimation()->play();
                }

                currentAnimation()->think(deltaTime);
            }
        }

        Animation* AnimationQueue::currentAnimation() const
        {
            return _animations.at(_currentAnimation).get();
        }

        Size AnimationQueue::size() const
        {
            return currentAnimation()->size();
        }

        Point AnimationQueue::offset() const
        {
            return currentAnimation()->offset();
        }

        Event::Handler& AnimationQueue::animationEndedHandler()
        {
            return _animationEndedHandler;
        }

        bool AnimationQueue::opaque(const Point &pos) {
            currentAnimation()->setPosition(this->position());
            return currentAnimation()->opaque(pos);
        }
    }
}
