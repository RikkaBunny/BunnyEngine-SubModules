#pragma once

namespace BE {

	class Timestep {
	public:
		//Timestep(float time = 0.0f)
		//	
		//{
		//	SetSeconds(time);
		//}
		inline operator float() { return m_Time; }

		inline static void SetSeconds(float time = 0.0f) { m_Time = time; }
		inline static float GetSeconds()  { return m_Time; }
		inline static float GetMilliseconds()  { return m_Time * 1000.0f; }
	private:
		static float m_Time;
		
	};

}