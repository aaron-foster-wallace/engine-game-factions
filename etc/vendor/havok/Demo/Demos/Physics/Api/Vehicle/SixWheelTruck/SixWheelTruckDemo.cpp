/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>


#include <Demos/Physics/Api/Vehicle/SixWheelTruck/SixWheelTruckDemo.h>
#include <Demos/Physics/Api/Vehicle/SixWheelTruck/SixWheelTruckSetup.h>

#include <Demos/Physics/Api/Vehicle/VehicleApi/VehicleApiUtils.h>

// We will need these shapes.
#include <Demos/DemoCommon/Utilities/GameUtils/Landscape/FlatLand.h>
#include <Physics/Vehicle/Camera/hkp1dAngularFollowCam.h>

#include <Common/Internal/ConvexHull/hkGeometryUtility.h>

#include <Common/Internal/ConvexHull/hkGeometryUtility.h>

// Need to build and update "wheel" display objects.
#include <Physics/Utilities/VisualDebugger/Viewer/hkpShapeDisplayBuilder.h>
#include <Graphics/Bridge/DisplayHandler/hkgDisplayHandler.h>
#include <Common/Visualize/Shape/hkDisplayGeometry.h>
#include <Common/Visualize/hkDebugDisplay.h>
#include <Graphics/Common/Window/hkgWindow.h>

// Vehicle specific includes.

#include <Physics/Utilities/Actions/Reorient/hkpReorientAction.h>

SixWheelTruckDemo::SixWheelTruckDemo(hkDemoEnvironment* env)
:	CarDemo(env, false, 6, 1)
{
	m_world->lock();
	{
		//
		// Create vehicle's chassis shape.
		//
		hkpConvexVerticesShape* chassisShape = HK_NULL;
		{
			hkReal xSize = 1.6f;
			hkReal ySize = 0.5f;
			hkReal zSize = 1.0f;

			hkReal xBumper = 1.9f;
			hkReal yBumper = 0.35f;
			hkReal zBumper = 1.0f;

			// 16 = 4 (size of "each float group", 3 for x,y,z, 1 for padding) * 4 (size of float).
			int stride = sizeof(float) * 4;
			
			{
				int numVertices = 12;

				float vertices[] = { 
					xSize, ySize, zSize, 0.0f,		// v0
					xSize, ySize, -zSize, 0.0f,		// v1
					xSize, -ySize, zSize, 0.0f,		// v2
					xSize, -ySize, -zSize, 0.0f,	// v3
					-xSize, ySize, zSize, 0.0f,		// v4
					-xSize, ySize, -zSize, 0.0f,	// v5
					-xSize, -ySize, zSize, 0.0f,	// v4
					-xSize, -ySize, -zSize, 0.0f,	// v5

					xBumper, yBumper, zBumper, 0.0f,	// v6
					xBumper, yBumper, -zBumper, 0.0f,	// v7
					-xBumper, yBumper, zBumper, 0.0f,	// v8
					-xBumper, yBumper, -zBumper, 0.0f,	// v9
				};
				
				//
				// SHAPE CONSTRUCTION.
				//
				hkArray<hkVector4> planeEquations;
				hkGeometry geom;
				{
					hkStridedVertices stridedVerts;
					{
						stridedVerts.m_numVertices = numVertices;
						stridedVerts.m_striding = stride;
						stridedVerts.m_vertices = vertices;
					}

					hkGeometryUtility::createConvexGeometry( stridedVerts, geom, planeEquations );

					{
						stridedVerts.m_numVertices = geom.m_vertices.getSize();
						stridedVerts.m_striding = sizeof(hkVector4);
						stridedVerts.m_vertices = &(geom.m_vertices[0](0));
					}

					chassisShape = new hkpConvexVerticesShape(stridedVerts, planeEquations);
				}
				chassisShape->setRadius(0.05f);
			}
		}

		hkReal wheelRadius = 0;
		hkReal wheelWidth = 0;

		for (int vehicleId = 0; vehicleId < m_numVehicles; vehicleId++)
		{
			//
			// Create the vehicle.
			//
			{
				//
				// Create the chassis body.
				//
				hkpRigidBody* chassisRigidBody;
				{
					hkpRigidBodyCinfo chassisInfo;

					chassisInfo.m_mass = 5000.0f;	
					chassisInfo.m_shape = chassisShape;
					chassisInfo.m_friction = 0.8f;
					chassisInfo.m_motionType = hkpMotion::MOTION_BOX_INERTIA;
					// Position chassis on the ground.
					chassisInfo.m_position.set(-40.0f, -4.5f, vehicleId * 5.0f);
					hkpInertiaTensorComputer::setShapeVolumeMassProperties(chassisInfo.m_shape,
																			chassisInfo.m_mass,
																			chassisInfo);

					chassisRigidBody = new hkpRigidBody(chassisInfo);

					m_world->addEntity(chassisRigidBody);

					HK_SET_OBJECT_COLOR((hkUlong)chassisRigidBody->getCollidable(), 0x80ff8080);
				}

				// This hkpAction flips the car upright if it turns over. 	 
				if (vehicleId == 0) 	 
				{ 	 
					hkVector4 rotationAxis(1.0f, 0.0f, 0.0f); 	 
					hkVector4 upAxis(0.0f, 1.0f, 0.0f); 	 
					hkReal strength = 15.0f; 	 
					m_reorientAction = new hkpReorientAction(chassisRigidBody, rotationAxis, upAxis, strength); 	 
				}

				SixWheelTruckSetup setup;
				m_vehicles[vehicleId].m_vehicle = createVehicle( setup, chassisRigidBody);
				m_vehicles[vehicleId].m_lastRPM = 0.0f;
				
				wheelRadius = m_vehicles[vehicleId].m_vehicle->m_data->m_wheelParams[0].m_radius;
				wheelWidth = m_vehicles[vehicleId].m_vehicle->m_data->m_wheelParams[0].m_width;

				chassisRigidBody->removeReference();
			}
		}
		HK_ASSERT(0x568a9221, wheelRadius!=0 && wheelWidth!=0);
		createDisplayWheels(wheelRadius, wheelWidth);
		chassisShape->removeReference();
	}

	//
	// Create the camera.
	//
	{
		VehicleApiUtils::createCamera( m_camera );
	}
	m_world->unlock();
}


#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = "Controls:\n" \
"\224\225 - accelerate / brake " \
"\226\227 - steer\n" \
"\221 - handbrake " \
"\223 - brake / reverse " \
"\235 - reorient\n"
"Turn right from start position to find 'icy patch'.";

HK_DECLARE_DEMO(SixWheelTruckDemo, HK_DEMO_TYPE_PHYSICS, "Drive a very heavy six-wheeled truck on a MOPP landscape", helpString );


/*
* Havok SDK - NO SOURCE PC DOWNLOAD, BUILD(#20090216)
* 
* Confidential Information of Havok.  (C) Copyright 1999-2009
* Telekinesys Research Limited t/a Havok. All Rights Reserved. The Havok
* Logo, and the Havok buzzsaw logo are trademarks of Havok.  Title, ownership
* rights, and intellectual property rights in the Havok software remain in
* Havok and/or its suppliers.
* 
* Use of this software for evaluation purposes is subject to and indicates
* acceptance of the End User licence Agreement for this product. A copy of
* the license is included with this software and is also available at www.havok.com/tryhavok.
* 
*/