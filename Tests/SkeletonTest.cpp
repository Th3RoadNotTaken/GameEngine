#include "Tests.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/string_cast.hpp"
#include "glm/gtx/euler_angles.hpp"

#include <iostream>

namespace Fiea::Engine::Tests
{
    using namespace glm;

    // x, y, z, w - typically used to translate a point through multiplication with a 4x4 transform
    using Vec4 = f32vec4;

    // x, y, z - used for positional information, also for rotation as euler angles where each value is the rotation around that axis
    using Vec3 = f32vec3;

    // 4x4 matrix is the standard used for rotation matrices and tranforms in most 3d implementations
    using Mat4x4 = f32mat4x4;

    /// @brief This enumeration provides the bones for our skeleton 
    enum class BoneSegment : size_t
    {
        // root node is on the ground directly under the hips
        ROOT,

        // primary backbone
        PELVIS,
        BACK,
        HEAD,

        // arms, attached at PELVIS
        L_SHOULDER,
        L_UPPERARM,
        L_FOREARM,

        R_SHOULDER,
        R_UPPERARM,
        R_FOREARM,

        // legs, attached at PELVIS
        L_HIP,
        L_THIGH,
        L_CALF,

        R_HIP,
        R_THIGH,
        R_CALF,

        COUNT,
        NONE = COUNT
    };

    /// @brief Bone is the heart of this system. Each bone can be visualized as a ball joint, which is anchored
    ///         at the terminal end of a "Parent" bone and oriented in the specified direction, and with a rod/bone of
    ///         the specified length extending from it. Bones may only have a single "Parent," but many may be parented
    ///         to a shared parent (note PELVIS in the data structure below, which has 3 bones extending from it)
    struct Bone
    {
        /// @brief An enumeration for the bone segment that this bone will extend from
        BoneSegment Parent;

        /// @brief The length of the straight rod segment of the bone
        float_t     Length;

        /// @brief The rotation, in euler angles, expressed as radians
        Vec3        Joint;
    };

    /// @brief Basic skeleton, which is just a collection of bones that will form a tree extending from the root
    struct Skeleton
    {
        static const size_t NUM_BONES = (size_t) BoneSegment::COUNT;
        Bone Bones[NUM_BONES];
    };

    /// @brief This is a data structure containing joint rotations to shift the skeleton from it's default T-pose into another pose
    struct AnimationFrame
    {
        static const size_t NUM_JOINTS = (size_t)BoneSegment::COUNT;

        /// @brief These deltas are additional rotation to be applied at each joint
        Vec3    JointDelta[NUM_JOINTS];

        /// @brief A multiplier for the length of the "bone" from the root to the pelvis. This is the only bone we will modify the
        ///         length of, because it is not a real bone, but rather a displacement from the ground under the skeleton to the
        ///         skeleton's pelvis
        float_t PelvicScale = 1.0f;
    };

    /// @brief A definition for a minimal skeleton
    static const Skeleton HumanoidSkeleton =
    {
        {
            { BoneSegment::NONE,         0.0f, { 0.0f, 0.0f, 0.0f } },                  // ROOT
            { BoneSegment::ROOT,         0.8f, { 0.0f, 0.0f, glm::radians(-90.0f) } },  // PELVIS
            { BoneSegment::PELVIS,       0.8f, { 0.0f, 0.0f, 0.0f } },                  // BACK
            { BoneSegment::BACK,         0.3f, { 0.0f, 0.0f, 0.0f } },                  // HEAD
            { BoneSegment::BACK,         0.2f, { 0.0f, 0.0f, glm::radians(90.0f) } },   // L_SHOULDER
            { BoneSegment::L_SHOULDER,   0.3f, { 0.0f, 0.0f, glm::radians(10.0f) } },   // L_UPPERARM
            { BoneSegment::L_UPPERARM,   0.3f, { 0.0f, 0.0f, 0.0f } },                  // L_FOREARM
            { BoneSegment::BACK,         0.2f, { 0.0f, 0.0f, glm::radians(-90.0f) } },  // R_SHOULDER
            { BoneSegment::R_SHOULDER,   0.3f, { 0.0f, 0.0f, glm::radians(-10.0f) } },  // R_UPPERARM
            { BoneSegment::R_UPPERARM,   0.3f, { 0.0f, 0.0f, 0.0f } },                  // R_FOREARM
            { BoneSegment::PELVIS,       0.1f, { 0.0f, 0.0f, glm::radians(90.0f) } },   // L_HIP
            { BoneSegment::L_HIP,        0.4f, { 0.0f, 0.0f, glm::radians(80.0f) } },   // L_THIGH
            { BoneSegment::L_THIGH,      0.4f, { 0.0f, 0.0f, 0.0f } },                  // L_CALF
            { BoneSegment::PELVIS,       0.1f, { 0.0f, 0.0f, glm::radians(-90.0f) } },  // R_HIP
            { BoneSegment::R_HIP,        0.4f, { 0.0f, 0.0f, glm::radians(-80.0f) } },  // R_THIGH
            { BoneSegment::R_THIGH,      0.4f, { 0.0f, 0.0f, 0.0f } }                   // R_CALF
        }
    };

    /// @brief A frame of animation to pose our skeleton with a slight turn at its waist
    static const AnimationFrame TurnAtWaist = {
        {
            { 0.0f, 0.0f, 0.0f },                   // ROOT
            { 0.0f, 0.0f, 0.0f },                   // PELVIS
            { glm::radians(30.0f), 0.0f, 0.0f },    // BACK
            { 0.0f, 0.0f, 0.0f },                   // HEAD
            { 0.0f, 0.0f, 0.0f },                   // L_SHOULDER
            { 0.0f, 0.0f, 0.0f },                   // L_UPPERARM
            { 0.0f, 0.0f, 0.0f },                   // L_FOREARM
            { 0.0f, 0.0f, 0.0f },                   // R_SHOULDER
            { 0.0f, 0.0f, 0.0f },                   // R_UPPERARM
            { 0.0f, 0.0f, 0.0f },                   // R_FOREARM
            { 0.0f, 0.0f, 0.0f },                   // L_HIP
            { 0.0f, 0.0f, 0.0f },                   // L_THIGH
            { 0.0f, 0.0f, 0.0f },                   // L_CALF
            { 0.0f, 0.0f, 0.0f },                   // R_HIP
            { 0.0f, 0.0f, 0.0f },                   // R_THIGH
            { 0.0f, 0.0f, 0.0f }                    // R_CALF
        },
        1.0f                                        // PelvicScale
    };

    /// @brief A frame of animation to pose our skeleton in a superhero landing
    static const AnimationFrame SuperheroLanding = { 
        {
            { 0.0f, 0.0f, 0.0f },                                   // ROOT
            { 0.0f, 0.0f, 0.0f },                                   // PELVIS
            { glm::radians(-20.0f), glm::radians(40.0f), 0.0f },    // BACK
            { 0.0f, glm::radians(40.0f), 0.0f},                     // HEAD
            { 0.0f, 0.0f, 0.0f },                                   // L_SHOULDER
            { 0.0f, glm::radians(20.0f), glm::radians(30.0f) },     // L_UPPERARM
            { 0.0f, 0.0f, 0.0f },                                   // L_FOREARM
            { glm::radians(60.f), 0.0f, 0.0f},                      // R_SHOULDER
            { glm::radians(-45.0f), glm::radians(60.0f), 0.0f},     // R_UPPERARM
            { 0.0f, glm::radians(60.0f), 0.0f},                     // R_FOREARM
            { 0.0f, glm::radians(-10.0f), 0.0f },                   // L_HIP
            { 0.0f, glm::radians(-20.0f), 0.0f},                    // L_THIGH
            { 0.0f, glm::radians(-20.0f), 0.0f},                    // L_CALF
            { 0.0f, glm::radians(10.0f), glm::radians(15.0f) },     // R_HIP
            { glm::radians(-40.0f), glm::radians(45.0f), glm::radians(20.0f)  },    // R_THIGH
            { 0.0f, glm::radians(-30.0f), 0.0f}                     // R_CALF
        },
        0.75f                                                       // PelvicScale
    };

    TEST_CLASS(SkeletonTest)
    {
    public:

        /// @brief This test case simply outputs the skeleton in world coordinates
        TEST_METHOD(TPose)
        {
            Vec3 worldPositions[Skeleton::NUM_BONES]{};
            Skeleton skeleton = HumanoidSkeleton;

            LocalToWorld(skeleton, worldPositions, std::size(worldPositions));
            OutputForDesmos(skeleton, worldPositions, std::size(worldPositions));
        }

        /// @brief In this test, a single joint animation is applied to rotate the character at the waist
        TEST_METHOD(SimpleAnim)
        {
            Vec3 worldPositions[Skeleton::NUM_BONES]{};
            Skeleton skeleton = HumanoidSkeleton;

            ApplyAnimation(skeleton, TurnAtWaist);
            LocalToWorld(skeleton, worldPositions, std::size(worldPositions));
            OutputForDesmos(skeleton, worldPositions, std::size(worldPositions));
        }

        /// @brief An entire animation frame, producing a "programmer art" version of a superhero landing pose
        TEST_METHOD(FullAnim)
        {
            Vec3 worldPositions[Skeleton::NUM_BONES]{};
            Skeleton skeleton = HumanoidSkeleton;

            ApplyAnimation(skeleton, SuperheroLanding);
            LocalToWorld(skeleton, worldPositions, std::size(worldPositions));
            OutputForDesmos(skeleton, worldPositions, std::size(worldPositions));
        }


        /// @brief A helper function to convert from euler angles into a rotation matrix
        /// @param eulers The euler angles to produce a rotation matrix from 
        /// @return The matrix
        static Mat4x4 EulerToRotation(const Vec3& eulers)
        {
            // TODO - You'll very likely need this method
            return glm::eulerAngleXYZ(eulers.x, eulers.y, eulers.z);
        }

        /// @brief A helper function to convert from a rotation matrix into euler angles
        /// @param matrix A rotation matrix to extract euler angles from 
        /// @return The euler angles, expressed as a vector
        static Vec3 RotationToEuler(const Mat4x4& matrix)
        {
            // TODO - You'll very likely need this method
            float x, y, z;
            glm::extractEulerAngleXYZ(matrix, x, y, z);
            return Vec3(x, y, z);
        }

        /// @brief Converts the skeletons local definition into a sequence of positions where
        ///         each bone terminates, in world coordinates
        /// @param skelly The skeleton to extract positional information from
        /// @param worldPositions An output array for the positional data
        /// @param positionCount The size of the output array
        static void LocalToWorld(const Skeleton& skelly, Vec3* worldPositions, size_t positionCount)
        {
            FIEA_ASSERT(positionCount == Skeleton::NUM_BONES);

            // The rotational data in the skeleton is provided such that each joint contains "local"
            //  coordinate system information, such that the rod extending from the joint extending
            //  along the +x axis.
            //
            // TODO - complete this method so that it populates the provided array of world positions
            //          with the terminal point for each bone. Assume the root bone is attached to the
            //          world at (0,0,0) and no incoming rotation

            // Each bone's world transforms
            std::vector<Mat4x4> worldMatrices(Skeleton::NUM_BONES);

            // Root bone is placed at identity
            worldMatrices[(size_t)BoneSegment::ROOT] = Mat4x4(1.0f);

            for (size_t i = 1; i < Skeleton::NUM_BONES; ++i)
            {
                const Bone& bone = skelly.Bones[i];
                size_t parent = (size_t)bone.Parent;

                Mat4x4 rotationMat = EulerToRotation(Vec3(bone.Joint.x, bone.Joint.y, bone.Joint.z));

                Mat4x4 translationMat = glm::translate(Mat4x4(1.0f), Vec3(bone.Length, 0.0f, 0.0f));

                // Local bone transform
                Mat4x4 localTransform = rotationMat * translationMat;

                worldMatrices[i] = worldMatrices[parent] * localTransform;
            }

            for (size_t i = 0; i < Skeleton::NUM_BONES; ++i)
            {
                Vec4 endPoint = worldMatrices[i] * Vec4(0.0f, 0.0f, 0.0f, 1.0f);
                worldPositions[i] = Vec3(endPoint.x, endPoint.y, endPoint.z);
            }

            glm::vec4 pelvisEnd = worldMatrices[size_t(BoneSegment::PELVIS)]
                * glm::vec4(0, 0, 0, 1);
        }

        /// @brief Modifies the provided skeleton by applying the animation frame to it
        /// @param skelly The skeleton to modify
        /// @param frame The animation data
        static void ApplyAnimation(Skeleton& skelly, const AnimationFrame& frame)
        {
            FIEA_ASSERT(skelly.NUM_BONES == frame.NUM_JOINTS);

            // TODO - complete this method so that it applies the rotation information in the
            //          frame onto the skeleton. The updated values should be directly applied
            //          to the provided skeleton.
            //
            //        Notes:
            //          * You cannot add euler angles (Hint: look this up)
            //          * The skeleton's rotation values are in local space before, during and after applying the animation
        
            for (size_t i = 0; i < Skeleton::NUM_BONES; ++i)
            {
                if (i == (size_t)BoneSegment::PELVIS)
                {
                    skelly.Bones[i].Length *= frame.PelvicScale;
                }

                Vec3 originalEuler = skelly.Bones[i].Joint;
                Vec3 animDeltaEuler = frame.JointDelta[i];

                // Make quaterions and compose angles
                glm::quat originalQuat = glm::quat(EulerToRotation(originalEuler));
                glm::quat animDeltaQuat = glm::quat(EulerToRotation(animDeltaEuler));

                glm::quat newQuat = animDeltaQuat * originalQuat;

                skelly.Bones[i].Joint = glm::eulerAngles(newQuat);
            }
        }

        /// @brief Outputs the joint positions, organized either as a collection of points or a series of vectors between
        ///         two points, or both
        /// @param skelly The skeleton, for the parentage information required to draw bones
        /// @param worldPositions The position of the terminal point for each bone
        /// @param positionCount The number of entries in the positions array (which should match the number of bones in the skelly)
        static void OutputForDesmos(const Skeleton& skelly, const Vec3* worldPositions, size_t positionCount)
        {
            const bool DRAW_BONES = true;
            const bool DRAW_JOINTS = false;

            // we should have 1 position per bone/joint
            FIEA_ASSERT(Skeleton::NUM_BONES == positionCount);

            if (DRAW_JOINTS)
            {
                // draw joint locations
                for (size_t i = 0; i < positionCount; ++i)
                {
                    const Vec3& pos = worldPositions[i];
                    LOG("p_{" << i << "} = (" << std::fixed << pos.x << ',' << pos.y << ',' << pos.z << ')' << std::endl);
                }

                // draw bones, relative to points
                if (DRAW_BONES)
                {
                    for (size_t i = 0; i < positionCount; ++i)
                    {
                        const Bone& bone = skelly.Bones[i];
                        size_t parent = (size_t)bone.Parent;

                        if (parent != (size_t)BoneSegment::NONE)
                        {
                            LOG("v_{" << i << "} = \\operatorname{vector}(p_{" << i << "}, p_{" << parent << "})" << std::endl);
                        }
                    }
                }
            }
            else
            {
                // if neither of these is true, then what are we doing?
                FIEA_ASSERT(DRAW_BONES);

                // draw bones
                for (size_t i = 0; i < positionCount; ++i)
                {
                    const Bone& bone = skelly.Bones[i];
                    const Vec3& pos = worldPositions[i];

                    size_t parent = (size_t)bone.Parent;
                    const Vec3& parentPos = worldPositions[parent];

                    if (parent != (size_t)BoneSegment::NONE)
                    {
                        LOG("v_{" << i << "} ="
                            "\\operatorname{vector}(" << std::fixed <<
                                '(' << pos.x << ',' << pos.y << ',' << pos.z << ")," <<
                                '(' << parentPos.x << ',' << parentPos.y << ',' << parentPos.z << ')' <<
                            ')' << std::endl
                        );
                    }
                }
            }
        }
    };
}
