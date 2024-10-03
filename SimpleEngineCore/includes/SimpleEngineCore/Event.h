#pragma once
#include <functional>
#include <array>

namespace SimpleEngine {

	enum class EventType {
		WindowResize = 0,
		WindowClose,

		KeyPressed,
		KeyReleased,

		MouseButtonPressed,
		MouseButtonReleased,
		MouseMoved,

		EventsCount
	};


	//=========== BaseEvent ===========//
	struct BaseEvent {
		BaseEvent() = default;
		virtual ~BaseEvent() = default;
		virtual EventType GetType() const = 0;
	};

	//=========== EventDispatcher ===========//
	class EventDispatcher {
	private:
		std::array<std::function<void(BaseEvent&)>, static_cast<size_t>(EventType::EventsCount)> eventCallbacks_;


	public:
		template <typename EventType>
		void AddEventListener(std::function<void(EventType&)> callback) {
			// создаетс€ функци€, котора€ будет вызыватьс€ при определенном ивенте
			auto baseCallback = [func = std::move(callback)](BaseEvent& e) {
				// func = std::move(callback) - насколько € пон€л, создаетс€
				// просто переменна€ где хранитс€ сам callback
				if (e.GetType() == EventType::type) {
					// здесь просто кастуетс€ наследник к базовому классу,
					// так как в array хран€тс€ BaseEvent&, и потом вызываетс€ переданный callback
					func(static_cast<EventType&>(e));
				}
			};
			eventCallbacks_[static_cast<size_t>(EventType::type)] = std::move(baseCallback);
		}

		void Dispatch(BaseEvent& event) {
			// приходит ивент, его тип кастуетс€ в size_t, чтобы искать в array функцию
			auto& callback = eventCallbacks_[static_cast<size_t>(event.GetType())];
			if (callback) {
				callback(event);
			}
		}
	};

	//=========== EventMouseMoved ===========//
	struct EventMouseMoved : public BaseEvent {
	public:
		double x, y;
		static const EventType type = EventType::MouseMoved;

	public:
		EventMouseMoved(double new_x, double new_y)
			: x(new_x)
			, y(new_y) {}

		virtual ~EventMouseMoved() = default;

		virtual EventType GetType() const override { return type; }

		
	};

	//=========== EventWindowResize ===========//
	struct EventWindowResize: public BaseEvent {
	public:
		unsigned int width;
		unsigned int height;
		static const EventType type = EventType::WindowResize;

	public:
		EventWindowResize(unsigned int new_width, unsigned int new_height)
			: width(new_width)
			, height(new_height) {}

		virtual ~EventWindowResize() = default;

		virtual EventType GetType() const override { return type; }

		
	};

	//=========== EventWindowClose ===========//
	struct EventWindowClose : public BaseEvent {
	public:
		static const EventType type = EventType::WindowClose;

	public:
		EventWindowClose() = default;
		virtual ~EventWindowClose() = default;

		virtual EventType GetType() const override { return type; }
	};


}