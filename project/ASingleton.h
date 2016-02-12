template <typename T> class ASingleton
{

public:
	static T& Instance() { if (_instance == nullptr) { _instance = new T(); }return _instance };
protected:
	static T _instance;

private:
	T& operator= (const T&) {};
};
