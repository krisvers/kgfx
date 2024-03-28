package com.krisvers.kgfx;

public class KGFXpipelinelayout {
	public KGFXpipelinebinding[] bindings;
	public KGFXdescriptorsetdesc[] descriptorSets;

	public KGFXpipelinelayout(KGFXpipelinebinding[] bindings, KGFXdescriptorsetdesc[] descriptorSets) {
		if (bindings == null) {
			throw new IllegalArgumentException("bindings must not be null");
		}
		if (descriptorSets == null) {
			throw new IllegalArgumentException("descriptorSets must not be null");
		}

		this.bindings = bindings;
		this.descriptorSets = descriptorSets;
	}

	public KGFXpipelinelayout() {
		this.bindings = new KGFXpipelinebinding[0];
		this.descriptorSets = new KGFXdescriptorsetdesc[0];
	}
}
