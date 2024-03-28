package com.krisvers.kgfx;

public class KGFXpipelinebinding {
	public int inputRate;
	public KGFXpipelineattribute[] attributes;
	public int bindpoint;
	public int binding;

	public KGFXpipelinebinding(int inputRate, KGFXpipelineattribute[] attributes, int bindpoint, int binding) {
		if (attributes == null) {
			throw new IllegalArgumentException("attributes must not be null");
		}

		this.inputRate = inputRate;
		this.attributes = attributes;
		this.bindpoint = bindpoint;
		this.binding = binding;
	}
}
