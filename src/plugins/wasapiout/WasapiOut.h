//////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2004-2019 musikcube team
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//    * Redistributions of source code must retain the above copyright notice,
//      this list of conditions and the following disclaimer.
//
//    * Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in the
//      documentation and/or other materials provided with the distribution.
//
//    * Neither the name of the author nor the names of other contributors may
//      be used to endorse or promote products derived from this software
//      without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
//////////////////////////////////////////////////////////////////////////////
#pragma once

#include "pch.h"

#include <deque>
#include <memory>
#include <mutex>
#include <atomic>

#include <mmdeviceapi.h>
#include <Audioclient.h>

#include <core/sdk/IOutput.h>
#include <core/sdk/IDevice.h>

using namespace musik::core::sdk;

class NotificationClient;

class WasapiOut : public IOutput {
    public:
        WasapiOut();
        ~WasapiOut();

        /* IPlugin */
        virtual const char* Name() { return "WASAPI"; };
        virtual void Release();

        /* IOutput */
        virtual void Pause() override;
        virtual void Resume() override;
        virtual void SetVolume(double volume) override;
        virtual double GetVolume() override;
        virtual void Stop() override;
        virtual int Play(IBuffer *buffer, IBufferProvider *provider) override;
        virtual double Latency() override;
        virtual void Drain() override;
        virtual IDeviceList* GetDeviceList() override;
        virtual bool SetDefaultDevice(const char* deviceId) override;
        virtual IDevice* GetDefaultDevice() override;

        void OnDeviceChanged() { this->deviceChanged = true; }

    private:
        enum State {
            StateStopped,
            StatePlaying,
            StatePaused
        };

        bool Configure(IBuffer *buffer);
        void Reset();
        IMMDevice* GetPreferredDevice();

        IMMDeviceEnumerator *enumerator;
        IMMDevice *device;
        IAudioClient *audioClient;
        IAudioClock *audioClock;
        IAudioRenderClient *renderClient;
        ISimpleAudioVolume *simpleAudioVolume;
        IAudioStreamVolume *audioStreamVolume;
        NotificationClient *notificationClient;
        UINT32 outputBufferFrames;
        std::atomic<State> state;
        WAVEFORMATEXTENSIBLE waveFormat;
        double volume;
        double latency;
        int rate;
        bool deviceChanged;
        std::recursive_mutex stateMutex;
};
