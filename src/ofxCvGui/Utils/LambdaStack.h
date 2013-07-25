#pragma once
#include <functional>
#include <map>

namespace ofxCvGui {
	namespace Utils {
		template<class ArgType>
		class LambdaStack{
			typedef std::function<void (ArgType&)> Functor;
			typedef uint32_t IndexType;
			struct Index {
				Index(IndexType order, void* owner) {
					this->order = order;
					this->owner = owner;
				}
				bool operator<(const Index& other) const {
					return this->order < other.order;
				}
				bool operator==(const IndexType& otherIndex) const {
					return this->order == otherIndex;
				}
				IndexType order;
				void* owner;
			};
			typedef std::map<Index, Functor> FunctorMap;
			typedef std::pair<Index, Functor> Pair;
		public:
			void operator+=(Functor functor) {
				this->addListener(functor, 0);
			}
			void addListener(Functor functor, void* owner) {
				auto last = this->listeners.end();
				last--;
				IndexType nextIndex = last->first.order + 1;
				this->listeners.insert(Pair(Index(nextIndex, owner), functor));
			}
			void addListener(Functor functor, int order, void* owner) {
				//loop until we find a free index
				while (listeners.count(index) > 0) {
					order++;
				}
				this->insert(Index(index, owner), functor);
			}
			void removeListeners(void* owner) {
				vector<IndexType> toRemove;
				for(auto iterator : this->listeners) {
					if (iterator.first.owner == owner) {
						toRemove.push_back(iterator.first.order);
					}
				}
				for(auto order : toRemove) {
					this->listeners.erase(Index(order, owner));
				}
			}
			void notifyListeners(ArgType& arguments) {
				for(auto listener : this->listeners) {
					listener.second(arguments);
				}
			}
			void operator()(ArgType& arguments) {
				this->notifyListeners(arguments);
			}
		protected:
			FunctorMap listeners;
			
		};
	}
}