namespace juce {
    namespace dsp {
        class BitShifter {
        public:
            BitShifter() noexcept = default;

            enum ShiftType {
                LeftShift,
                RightShift
            };

            //==============================================================================
            void setBitOffset(int newOffset) { 
                if (newOffset <= sampleSizeInBits) { bitOffset = newOffset; }
            }
            void setShiftType(juce::dsp::BitShifter::ShiftType newType) { shiftType = newType; }

            int getBitOffset() { return bitOffset; }
            juce::dsp::BitShifter::ShiftType setShiftType() { return shiftType; }

            //==============================================================================
            /** Called before processing starts. */
            void prepare(const ProcessSpec& spec) noexcept
            {
                reset();
            }

            /** Resets the internal state. */
            void reset() noexcept
            {
            }

            /* Update sample size in both bytes and bits. */
            template <typename SampleType>
            void UpdateSampleSize(SampleType s) {
                sampleSizeInBytes = sizeof(s);
                sampleSizeInBits = sampleSizeInBytes * 8;
            }

            //==============================================================================
            /** Returns the result of processing a single sample. */
            template <typename SampleType>
            SampleType JUCE_VECTOR_CALLTYPE processSample(SampleType s) noexcept
            {
                uint32_t bits;
                std::memcpy(&bits, &s, sampleSizeInBytes);
                if (shiftType == ShiftType::LeftShift) bits &= bits << bitOffset;
                else bits &= bits >> bitOffset;
                std::memcpy(&s, &bits, sampleSizeInBytes);
                return s;
            }

            /** Processes the input and output buffers supplied in the processing context. */
            template <typename ProcessContext>
            void process(const ProcessContext& context) noexcept
            {
                auto&& inBlock = context.getInputBlock();
                auto&& outBlock = context.getOutputBlock();

                jassert(inBlock.getNumChannels() == outBlock.getNumChannels());
                jassert(inBlock.getNumSamples() == outBlock.getNumSamples());

                auto len = inBlock.getNumSamples();
                auto numChannels = inBlock.getNumChannels();

                UpdateSampleSize(inBlock.getSample(0, 0));

                // Simply copy input to output if plugin is bypassed.
                if (context.isBypassed)
                {
                    if (context.usesSeparateInputAndOutputBlocks())
                        outBlock.copyFrom(inBlock);
                    return;
                }

                // Apply to every channel
                for (size_t chan = 0; chan < numChannels; chan++)
                {
                    auto* src = inBlock.getChannelPointer(chan);
                    auto* dst = outBlock.getChannelPointer(chan);

                    // Apply to every sample
                    for (int i = 0; i < len; i++)
                        dst[i] = processSample(src[i]);
                }
            }

        private:
            int sampleSizeInBytes = sizeof(float);
            int sampleSizeInBits = sizeof(float) * 8;
            ShiftType shiftType = ShiftType::LeftShift;
            int bitOffset;
        };
    }
}