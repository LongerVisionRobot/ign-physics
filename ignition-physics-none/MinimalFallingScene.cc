/*
 * Copyright (C) 2017 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/

#include "ignition/common/Console.hh"

#include "MinimalScenes.hh"

namespace ignition
{
  namespace physics
  {
    namespace none
    {
      class PrivateMinimalFallingScene
      {
        public: WorldPoses poses;

        public: std::size_t lastId;

        PrivateMinimalFallingScene()
          : lastId(0)
        {
          this->poses.annotation = "MinimalFallingScene initialized";

          for (unsigned int i = 0; i < 10; ++i)
          {
            WorldPose pose;
            pose.body = this->lastId++;
            pose.pose.Pos().X() = pose.body;
            pose.pose.Pos().Y() = pose.body * 4;
            pose.pose.Pos().Z() = pose.body * 9;
            this->poses.entries.push_back(pose);
          }
        }

        void WriteState(ForwardStep::State &_x)
        {
          WorldPoses &state = _x.Get<WorldPoses>();
          state = this->poses;
        }

        void Simulate()
        {
          for (auto &entry : this->poses.entries)
          {
            entry.pose.Pos().Z() -= 1.0;
          }
        }
      };

      MinimalFallingScene::~MinimalFallingScene()
      {
        // Do nothing
      }

      MinimalFallingScene::MinimalFallingScene()
        : dataPtr(new PrivateMinimalFallingScene)
      {
        // Do nothing
      }

      void MinimalFallingScene::Step(
          Output &_h, ForwardStep::State &_x, const Input &/*_u*/)
      {
        this->dataPtr->Simulate();

        this->dataPtr->WriteState(_x);

        _h.ResetQueries();
        this->WriteRequiredData(_h);
      }

      void MinimalFallingScene::SetStateTo(const SetState::State &_x)
      {
        const WorldPoses *state = _x.Query<WorldPoses>();
        if (!state)
        {
          ignerr << "[ignition::physics::none::MinimalFallingScene::"
                 << "SetState] The state provided does not contain a "
                 << "WorldPoses, which this plugins needs in order to go to "
                 << "a specified state!"
                 << std::endl;
          return;
        }

        this->dataPtr->poses = *state;
      }

      void MinimalFallingScene::Write(WorldPoses &_poses) const
      {
        _poses = this->dataPtr->poses;
      }
    }
  }
}