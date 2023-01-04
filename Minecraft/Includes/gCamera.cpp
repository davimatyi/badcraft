#include <iostream>
#include "gCamera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <math.h>

/// <summary>
/// Initializes a new instance of the <see cref="gCamera"/> class.
/// </summary>
gCamera::gCamera(void) : m_eye(0.0f, 20.0f, 20.0f), m_at(0.0f), m_up(0.0f, 1.0f, 0.0f), m_speed(4.317f), m_goFw(0), m_goRight(0), m_goUp(0), m_slow(false)
{
	SetView( glm::vec3(0,20,20), glm::vec3(0,0,0), glm::vec3(0,1,0));

	m_dist = glm::length( m_at - m_eye );	

	SetProj(45.0f, 640/480.0f, 0.001f, 1000.0f);
}

gCamera::gCamera(glm::vec3 _eye, glm::vec3 _at, glm::vec3 _up) : m_speed(16.0f), m_goFw(0), m_goRight(0), m_goUp(0), m_dist(10), m_slow(false)
{
	SetView(_eye, _at, _up);
}

gCamera::~gCamera(void)
{
}

void gCamera::SetView(glm::vec3 _eye, glm::vec3 _at, glm::vec3 _up)
{
	m_eye	= _eye;
	m_at	= _at;
	m_up	= _up;

	m_fw  = glm::normalize( m_at - m_eye  );
	m_st = glm::normalize( glm::cross( m_fw, m_up ) );

	m_dist = glm::length( m_at - m_eye );	

	m_u = atan2f( m_fw.z, m_fw.x );
	m_v = acosf( m_fw.y );
}

void gCamera::SetProj(float _angle, float _aspect, float _zn, float _zf)
{
	m_matProj = glm::perspective( _angle, _aspect, _zn, _zf);
	m_matViewProj = m_matProj * m_viewMatrix;
}

glm::mat4 gCamera::GetViewMatrix()
{
	return m_viewMatrix;
}

void gCamera::Update(float _deltaTime)
{
	m_eye += (m_goFw * glm::normalize(m_fw * glm::vec3(1, 0, 1)) + m_goRight * glm::normalize(m_st * glm::vec3(1, 0, 1)) + m_goUp * m_up ) * m_speed * _deltaTime;
	m_at  += (m_goFw * glm::normalize(m_fw * glm::vec3(1, 0, 1)) + m_goRight * glm::normalize(m_st * glm::vec3(1, 0, 1)) + m_goUp * m_up ) * m_speed * _deltaTime;

	m_viewMatrix = glm::lookAt( m_eye, m_at, m_up);
	m_matViewProj = m_matProj * m_viewMatrix;

	SDL_SetRelativeMouseMode(mouseCaptured ? SDL_TRUE : SDL_FALSE);
}

void gCamera::UpdateUV(float du, float dv)
{
	m_u		+= du;
	m_v		 = glm::clamp<float>(m_v + dv, 0.1f, 3.1f);

	m_at = m_eye + m_dist*glm::vec3(	cosf(m_u)*sinf(m_v), 
										cosf(m_v), 
										sinf(m_u)*sinf(m_v) );

	m_fw = glm::normalize( m_at - m_eye );
	m_st = glm::normalize( glm::cross( m_fw, m_up ) );
}

void gCamera::SetSpeed(float _val)
{
	m_speed = _val;
}

void gCamera::Resize(int _w, int _h)
{
	m_matProj = glm::perspective(	45.0f, _w/(float)_h, 0.01f, 1000.0f);

	m_matViewProj = m_matProj * m_viewMatrix;
}

void gCamera::KeyboardDown(SDL_KeyboardEvent& key)
{
	switch ( key.keysym.sym )
	{
	case SDLK_LSHIFT:
		m_goUp = -1;
		break;
	case SDLK_SPACE:
		m_goUp = 1;
		break;
	case SDLK_w:
		m_goFw = 1;
		break;
	case SDLK_s:
		m_goFw = -1;
		break;
	case SDLK_a:
		m_goRight = -1;
		break;
	case SDLK_d:
		m_goRight = 1;
		break;
	case SDLK_ESCAPE:
		mouseCaptured = !mouseCaptured;
		break;
	}
}

void gCamera::KeyboardUp(SDL_KeyboardEvent& key)
{
	float current_speed = m_speed;
	switch ( key.keysym.sym )
	{
	case SDLK_LSHIFT:
	case SDLK_SPACE:
		m_goUp = 0;
		break;
	case SDLK_w:
	case SDLK_s:
			m_goFw = 0;
		break;
	case SDLK_a:
	case SDLK_d:
			m_goRight = 0;
		break;
	}
}

void gCamera::MouseMove(SDL_MouseMotionEvent& mouse)
{
	if (mouseCaptured)
	{
		UpdateUV(mouse.xrel/600.0f, mouse.yrel/600.0f);
	}
}

void gCamera::LookAt(glm::vec3 _at)
{
	SetView(m_eye, _at, m_up);
}

